
#include "Volume.h"
#include "Engine/Core/Log.h"


void Volume::Init(VolumeConfig conf, MesherType mesherType)
{
	config = conf;
	octree = new Octree();
	mesher = new MC();
	cacheObserverPos = Vector3(0, 0, 0);

	if (config.parent->GetChildren<StaticModel>().Count() > 0) {
		config.parent->GetChildren<StaticModel>()[0]->DeleteObject();
	}
	
	Model* model = Content::CreateVirtualAsset<Model>();

	const int32* meshesCountPerLod = new int32[1]{ 1 };
	Span<int32> spanMeshesCountPerLod(meshesCountPerLod, sizeof(int32));
	model->SetupLODs(spanMeshesCountPerLod);

	mesh = &model->LODs[0].Meshes[0];

	auto sm = New<StaticModel>();
	sm->Model = model;
	sm->SetName(StringView(TEXT("GenMesh")));
	sm->SetForcedLOD(0);

	sm->SetParent(config.parent);
}


void Volume::FromSDF() {
	
	const auto sdfFunc = [](Vector3 pos) -> float {
		return SDF::Sphere(Vector3((float)pos.X, (float)pos.Y, (float)pos.Z), 100);
	};

	GenerateFromSDF(sdfFunc, config.origin, config.size); //_sdfFunc, 

	/*if (octree->Nodes.size() > 0) {
		octree->Visualize(&octree->Nodes[1]);
	}*/
}

void Volume::GenerateFromSDF(std::function<float(Vector3)> sdfFunc, Int3 origin, int size)
{
	SDFFunc = sdfFunc;
	mesher->CleanDrawData(true);
	CreateDrawData(sdfFunc, config.origin, config.size);
	mesher->CleanDrawDataCache();
	DrawVolume();
}

void Volume::CreateDrawData(std::function<float(Vector3)> sdfFunc, Int3 origin, int size, int nodeIndex, uint_fast64_t parentLocCode) {

	if (config.observerCam->GetFrustum().Contains(BoundingBox(
		Vector3(origin.X - size / 2, origin.Y - size / 2, origin.Z - size / 2),
		Vector3(origin.X + size / 2, origin.Y + size / 2, origin.Z + size / 2))) == ContainmentType::Disjoint) {
		octree->ClearByLocCode(parentLocCode, nodeIndex);
		return;
	}

	OctreeNode* node = octree->AddNode(OctreeNode{ size, origin }, parentLocCode, nodeIndex);
	node->isLeaf = false;

	float nodeCenterSurfDist = abs(sdfFunc(Vector3(origin.X, origin.Y, origin.Z)));

	if (nodeCenterSurfDist <= ((size * QRT3) / 2))
	{
		int newSize = size / 2;
		int maxLodRes = config.maxResolution;

		float observerNodeDist = Vector3::Distance(config.observerCam->GetPosition(), Vector3(origin.X, origin.Y, origin.Z)) - newSize;

		int multLodRes = 1;

		for (int n = 0; n < config.lodDistances.Count(); n++)
		{
			if (observerNodeDist > config.lodDistances[n]) {
				multLodRes += 1;
			}
		}

		maxLodRes *= multLodRes;

		if (newSize >= maxLodRes) {

			int newSizeSplit = newSize / 2;

			node->childExists = true;

			for (int n = 0; n < 8; n++)
			{
				Int3 shift = Int3(cubeNegVerts[n][0] * newSizeSplit, cubeNegVerts[n][1] * newSizeSplit, cubeNegVerts[n][2] * newSizeSplit);
				Int3 newOrigin = Int3(origin.X + shift.X, origin.Y + shift.Y, origin.Z + shift.Z);
				CreateDrawData(sdfFunc, newOrigin, newSize, n, node->locCode);
			}
		}
		else {
			octree->CleanChildren(node);
			node->isLeaf = true;

			float voxelCornerDensity[8] = { 0 };
			Vector3 voxelCornerNormal[8] = { 0 };

			Int3 zeroCornerPos;
			Int3 cornerPos[8];

			int sameAsCacheDensities = 0;
			bool hasDensityCache = node->cornerDensityCache.Count() > 0;

			for (int n = 0; n < 8; n++)
			{
				Int3 shift = Int3(cubeNegVerts[n][0] * newSize, cubeNegVerts[n][1] * newSize, cubeNegVerts[n][2] * newSize);
				cornerPos[n] = Int3(origin.X + shift.X, origin.Y + shift.Y, origin.Z + shift.Z);

				voxelCornerDensity[n] = sdfFunc(Vector3(cornerPos[n].X, cornerPos[n].Y, cornerPos[n].Z));

				if (hasDensityCache && node->cornerDensityCache[n] == voxelCornerDensity[n]) {
					sameAsCacheDensities++;
				}

				if (n == 0) {
					zeroCornerPos = cornerPos[n];
				}
			}

			/*if (sameAsCacheDensities == 8 && mesher->GetDrawDataCache() != nullptr) {
				int vertexCount = mesher->GetDrawData()->vertex.Count();

				for (int n = 0; n < node->cachedIndices.indices.Count(); n++)
				{
					VB0ElementType cachedVertex = mesher->GetDrawDataCache()->vertex[node->cachedIndices.indices[n]];
					VB1ElementType cachedNormalsTangents = mesher->GetDrawDataCache()->normalsTangents[node->cachedIndices.indices[n]];
					uint32 newIndiceIndex = vertexCount + node->cachedIndices.indices[n] - node->cachedIndices.minIndiceIndex;
					mesher->GetDrawData()->vertex.Add(cachedVertex);
					mesher->GetDrawData()->normalsTangents.Add(cachedNormalsTangents);
					mesher->GetDrawData()->indices.Add(newIndiceIndex);
				}
			}
			else {*/
				for (int n = 0; n < 8; n++)
				{
					float H = 0.001;

					float nx = sdfFunc(Vector3((float)cornerPos[n].X + H, cornerPos[n].Y, cornerPos[n].Z)) - sdfFunc(Vector3((float)cornerPos[n].X - H, cornerPos[n].Y, cornerPos[n].Z));
					float ny = sdfFunc(Vector3(cornerPos[n].X, (float)cornerPos[n].Y + H, cornerPos[n].Z)) - sdfFunc(Vector3(cornerPos[n].X, (float)cornerPos[n].Y - H, cornerPos[n].Z));
					float nz = sdfFunc(Vector3(cornerPos[n].X, cornerPos[n].Y, (float)cornerPos[n].Z + H)) - sdfFunc(Vector3(cornerPos[n].X, cornerPos[n].Y, (float)cornerPos[n].Z - H));

					float dlen = sqrt(nx * nx + ny * ny + nz * nz);

					if (dlen > 0.0) {
						float mul = 1.0 / dlen;
						nx *= mul;
						ny *= mul;
						nz *= mul;
					}

					voxelCornerNormal[n] = Vector3(nx, ny, nz);
				}

				node->cachedIndices = mesher->AddNode(voxelCornerDensity, zeroCornerPos, size, voxelCornerNormal);
			//}

			node->cornerDensityCache.Set(voxelCornerDensity, 8);
		}
	}
	else {
		octree->CleanChildren(node);
	}
}

void Volume::DrawVolume()
{	
	mesh->UpdateMesh(
		mesher->GetDrawData()->vertex.Count(),
		mesher->GetDrawData()->indices.Count() / 3,
		mesher->GetDrawData()->vertex.Get(),
		mesher->GetDrawData()->normalsTangents.Get(),
		nullptr,
		mesher->GetDrawData()->indices.Get()
	);
}

void Volume::UpdateObserverCam(Camera* observerCam)
{

	/*const auto sdfFunc = [](Vector3 pos) -> float {
		return SDF::Sphere(Vector3((float)pos.X, (float)pos.Y, (float)pos.Z), 100);
	};

	if (cacheObserverPos != observerCam->GetPosition()) {
		cacheObserverPos = observerCam->GetPosition();
		mesher->CleanDrawData(true);
		CreateDrawData(sdfFunc, config.origin, config.size);
		mesher->CleanDrawDataCache();
		DrawVolume();
	}*/
}
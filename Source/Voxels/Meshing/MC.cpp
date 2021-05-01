
#include "MC.h"
#include "Engine/Core/Collections/Array.h"
#include "Engine/Graphics/Models/Types.h"
#include "Engine/Core/Log.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Core/Math/BoundingSphere.h";
#include <algorithm>


void MC::CleanDrawData(bool cache) {
	if (cache && !empty) {
		//LOG(Info, "Copyyyyyyyyyyyyyyyyy");
		memcpy(drawDataCache, drawData, sizeof(DrawData));
		//drawDataCache = drawData;
	}

	if (!empty && drawData != NULL) {
		//Platform::Free(drawData);
		//LOG(Info, "Deleteeeeeeeeeeeeeeeeeeeeeeeee");
		//delete drawData;
		drawData = new DrawData();
		//drawData = new DrawData();
	}
}

void MC::CleanDrawDataCache()
{
	//if (drawDataCache != nullptr) {
		//Platform::Free(drawDataCache);
		//delete(drawDataCache);
		//drawDataCache = new DrawData();
	//}
	if (drawDataCache != NULL) {
		//LOG(Info, "Deleteeeeeeeeeeeeeeeeeeeeeeeee2");
		//delete drawDataCache;
		drawDataCache = new DrawData();
	}
}

DrawData* MC::GetDrawData() {
	return drawData;
}
DrawData* MC::GetDrawDataCache() {
	return drawDataCache;
}

NodeDrawIndicesData MC::AddNode(float cornerDensity[8], Int3 zeroCornerPos, int size, Vector3 voxelCornerNormal[8])
{	

	Array<uint32> nodeIndices;

	int cubeIndex = 0;
	//float grid[8] = { 0 };
	int edges[12] = { 0 };

	for (int n = 0; n < 8; n++)
	{
		float cd = cornerDensity[n];
		//grid[n] = cd;
		cubeIndex |= (cd > 0) ? 1 << n : 0;
	}

	const int edgeMask = edgeTable[cubeIndex];

	if (edgeMask == 0) {
		return NodeDrawIndicesData{ nodeIndices, (uint32)drawData->vertex.Count() - 1 };
	}

	for (int i = 0; i < 12; i++) {
		if ((edgeMask & (1 << i)) == 0) continue;

		edges[i] = drawData->vertex.Count();		
		const int* edge = edgeIndex[i];

		const int* p0 = cubeVerts[edge[0]];
		const int* p1 = cubeVerts[edge[1]];

		float a = cornerDensity[edge[0]];
		float b = cornerDensity[edge[1]];

		float d = a - b;
		float t = 0.0;

		if (abs(d) > 1e-6) t = a / d;

		Vector3 nv = Vector3();

		nv.X = zeroCornerPos.X + ((1 - t) * p0[0] + t * p1[0]) * size;
		nv.Y = zeroCornerPos.Y + ((1 - t) * p0[1] + t * p1[1]) * size;
		nv.Z = zeroCornerPos.Z + ((1 - t) * p0[2] + t * p1[2]) * size;

		//LOG(Info, "Node added {0}", nv);

		drawData->vertex.Add(VB0ElementType{ nv });

		Vector3 an = voxelCornerNormal[edge[0]];
		Vector3 bn = voxelCornerNormal[edge[1]];

		Vector3 normal = Vector3();

		normal.X = (1 - t) * an.X + t * bn.X;
		normal.Y = (1 - t) * an.Y + t * bn.Y;
		normal.Z = (1 - t) * an.Z + t * bn.Z;

		Vector3 tangent;
		Vector3 t1 = Vector3::Cross(normal, Vector3::Forward);
		Vector3 t2 = Vector3::Cross(normal, Vector3::Up);

		if (t1.Length() > t2.Length())
		{
			tangent = t1;
		}
		else
		{
			tangent = t2;
		}

		VB1ElementType vb1;

		Vector3 rn = normal * 0.5f + 0.5f;
		rn.Normalize();

		//LOG(Info, "norm = {0}", rn);

		vb1.Normal = Float1010102(rn);

		Vector3 bitangent = Vector3::Normalize(Vector3::Cross(normal, tangent));
		byte sign = static_cast<byte>(Vector3::Dot(Vector3::Cross(bitangent, normal), tangent) < 0.0f ? 1 : 0);

		//Vector3 tt = tangent * 0.5f + 0.5f;
		//tt.Normalize();

		vb1.Tangent = Float1010102(tangent * 0.5f + 0.5f, sign);

		drawData->normalsTangents.Add(vb1);

		DEBUG_DRAW_SPHERE(BoundingSphere(nv, 1), Color::Red, 20, true);
		DEBUG_DRAW_LINE(nv, nv + normal * 20, Color::Red, 20, true);
		DEBUG_DRAW_LINE(nv, nv + tangent * 20, Color::Green, 20, true);

	}

	std::vector<int> tris = triTable[cubeIndex];	
	uint32 minIndiceIndex = INFINITY;
	int i = 0;

	while (i < tris.size()) {

		uint32 i1 = edges[tris[i]];
		uint32 i2 = edges[tris[i+1]];
		uint32 i3 = edges[tris[i+2]];

		LOG(Info, "{0} , {1} , {2}", i1, i2, i3);

		drawData->indices.Add(i1);
		drawData->indices.Add(i2);
		drawData->indices.Add(i3);

		nodeIndices.Add(i1);
		nodeIndices.Add(i2);
		nodeIndices.Add(i3);

		uint32 iterMin = std::min(std::min(i1, i2), i3);

		if (iterMin < minIndiceIndex) {
			minIndiceIndex = iterMin;
		}

		i += 3;
	}

	empty = false;

	return NodeDrawIndicesData{ nodeIndices, minIndiceIndex };
}
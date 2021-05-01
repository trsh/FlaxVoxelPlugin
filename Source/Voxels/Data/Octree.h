
#pragma once

#include <unordered_map>
#include "Engine/Core/Math/Vector3.h";
#include "Engine/Core/Math/Int3.h";
#include "Engine/Core/Log.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Core/Math/BoundingBox.h"
#include "Engine/Core/Collections/Array.h"
#include "../Meshing/Meshing.h";

struct OctreeNode {
public:
	int size;
	Int3 position;
	uint_fast64_t locCode;	
	uint8_t childExists;
	bool isLeaf;
	Array<float> cornerDensityCache;
	NodeDrawIndicesData cachedIndices;
};

class Octree
{
public:
	void Visualize(OctreeNode* node);
	void CleanChildren(OctreeNode* node);

	std::unordered_map<uint_fast64_t, OctreeNode> Nodes;
	OctreeNode* AddNode(OctreeNode node, uint_fast64_t parentLocCode, uint_fast64_t childIndex)
	{	
		uint_fast64_t newLocCode = parentLocCode << 3 | childIndex;

		if (LookupNode(newLocCode) == nullptr) {
			node.locCode = newLocCode;
			Nodes[newLocCode] = node;			
		}

		return &Nodes[newLocCode];
	}

	void ClearByLocCode(uint_fast64_t parentLocCode, uint_fast64_t childIndex)
	{
		uint_fast64_t newLocCode = parentLocCode << 3 | childIndex;
		OctreeNode* node = LookupNode(newLocCode);

		if (node != nullptr) {
			CleanChildren(&Nodes[newLocCode]);
			Nodes.erase(newLocCode);
		}
	}	

	OctreeNode* GetParentNode(OctreeNode* node)
	{
		const uint_fast64_t locCodeParent = node->locCode >> 3;
		return LookupNode(locCodeParent);
	}

	OctreeNode* LookupNode(uint_fast64_t locCode)
	{
		const auto iter = Nodes.find(locCode);
		return (iter == Nodes.end() ? nullptr : &iter->second);
	}
};
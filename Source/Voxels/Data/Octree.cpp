
#include "Octree.h"
#include "Engine/Core/Log.h"

void Octree::Visualize(OctreeNode* node)
{
	DEBUG_DRAW_WIRE_BOX(BoundingBox(
		Vector3(node->position.X - node->size / 2, node->position.Y - node->size / 2, node->position.Z - node->size / 2),
		Vector3(node->position.X + node->size / 2, node->position.Y + node->size / 2, node->position.Z + node->size / 2)),
		Color::Red, 20, true);
	
	if (node->childExists)
	{
		for (int i = 0; i < 8; i++)
		{
			const uint32_t locCodeChild = (node->locCode << 3) | i;
			OctreeNode* child = LookupNode(locCodeChild);

			if (child != nullptr) {
				Visualize(child);
			}
		}
	}
}

void Octree::CleanChildren(OctreeNode* node)
{
	if (node->childExists)
	{
		for (int i = 0; i < 8; i++)
		{
			const uint32_t locCodeChild = (node->locCode << 3) | i;
			OctreeNode* child = LookupNode(locCodeChild);

			if (child != nullptr) {
				CleanChildren(child);
				Nodes.erase(locCodeChild);				
			}
		}

		node->childExists = false;
	}
}
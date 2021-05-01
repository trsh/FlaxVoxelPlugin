
#pragma once

#include "Engine/Scripting/Script.h"
#include "../Meshing/MeshingData.h"

struct NodeDrawIndicesData {
public:
    Array<uint32> indices;
    uint32 minIndiceIndex;
};

API_ENUM() enum class VOXELS_API MesherType {
    MC = 1
};


API_CLASS(Abstract) class VOXELS_API Mesher : public Script {
    DECLARE_SCRIPTING_TYPE_MINIMAL(Mesher);
public:
    virtual DrawData* GetDrawData() = 0;
    virtual DrawData* GetDrawDataCache() = 0;
    virtual NodeDrawIndicesData AddNode(float cornerDensity[8], Int3 zeroCornerPos, int size, Vector3 voxelCornerNormal[8]) = 0;
    virtual void CleanDrawData(bool cache = true) = 0;
    virtual void CleanDrawDataCache() = 0;
};
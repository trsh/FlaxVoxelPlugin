
#pragma once

#include <functional>
#include <unordered_map>

#include "Engine/Scripting/Script.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Core/Math/BoundingBox.h"
#include "Engine/Core/Math/Int3.h"
#include "Engine/Core/Collections/Array.h"

#include <Engine/Level/Actors/StaticModel.h>
#include <Engine/Level/Actors/Camera.h>
#include <Engine/Level/Level.h>
#include <Engine/Level/Scene/Scene.h>
#include <Engine/Content/Assets/MaterialBase.h>
#include <Engine/Content/Assets/Model.h>
#include <Engine/Content/Content.h>
#include <Engine/Core/Types/Span.h>
#include <Engine/Core/Delegate.h>
#include <Engine/Core/Types/CommonValue.h>
#include <Engine/Content/Assets/Material.h>

#include "../Math/SDF.h"
#include "../Data/Octree.h";
#include "../Meshing/MC.h";
#include "../Meshing/Meshing.h";
#include "../Data/libmorton/morton.h";


const float QRT3 = sqrt(3);

API_STRUCT() struct VOXELS_API VolumeConfig {
    DECLARE_SCRIPTING_TYPE_MINIMAL(VolumeConfig);
    API_FIELD() Int3 origin;
    API_FIELD() int32 maxResolution;
    API_FIELD() int64 size;
    API_FIELD() Camera *observerCam;
    API_FIELD() Actor* parent;
    API_FIELD() Array<int32> lodDistances;    
};

API_CLASS() class VOXELS_API Volume : public Script
{
DECLARE_SCRIPTING_TYPE_NO_SPAWN(Volume);

public:        
    API_FUNCTION() void Init(VolumeConfig config, MesherType mesherType);
    API_FUNCTION() void UpdateObserverCam(Camera *observerCam);        

    // TODO wait for support of callback
    void GenerateFromSDF(std::function<float(Vector3)> sdfFunc, Int3 origin, int size);

    //Test function, TODO remove
    API_FUNCTION() void FromSDF();

    VolumeConfig config;

private:    
    std::function<float(Vector3)> SDFFunc;
    Vector3 cacheObserverPos;    
    Octree *octree;
    Mesher *mesher;
    Mesh *mesh;
    void DrawVolume();    
    void CreateDrawData(std::function<float(Vector3)> sdfFunc, Int3 origin, int size, int index = 1, uint_fast64_t parentLocCode = 0b0);
};
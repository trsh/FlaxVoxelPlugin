#pragma once

#include "Engine/Core/Math/Vector3.h";
#include "Engine/Scripting/Script.h"

/// <summary>
/// Describes SDF functions.
/// </summary>
API_CLASS() class VOXELS_API SDF : public Script
{

DECLARE_SCRIPTING_TYPE_NO_SPAWN(SDF);
    
public:
    /// <summary>
    /// Sphere SDF.
    /// </summary>
    API_FUNCTION() static float Sphere(Vector3 p, float r);
};

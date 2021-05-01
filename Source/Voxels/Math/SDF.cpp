
#include "SDF.h"
#include "Engine/Core/Log.h"

float SDF::Sphere(Vector3 p, float r)
{
	return p.Length() - r;
}
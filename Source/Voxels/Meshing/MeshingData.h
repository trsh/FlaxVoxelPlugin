
#pragma once

#include "Engine/Scripting/Script.h"
#include "Engine/Core/Collections/Array.h"
#include "Engine/Graphics/Models/Types.h"

/// <summary>
/// Volume draw data.
/// </summary>
struct DrawData {
public:
	/// <summary>
	/// Draw vertices.
	/// </summary>
	Array<VB0ElementType> vertex;
	/// <summary>
	/// Draw indices.
	/// </summary>
	Array<uint32> indices;
	/// <summary>
	/// Draw tangent & normal vectors.
	/// </summary>
	Array<VB1ElementType> normalsTangents;
};


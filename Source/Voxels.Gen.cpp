// This code was auto-generated. Do not modify it.

#include "Engine/Scripting/BinaryModule.h"
#include "Voxels.Gen.h"

StaticallyLinkedBinaryModuleInitializer StaticallyLinkedBinaryModuleVoxels(GetBinaryModuleVoxels);

extern "C" BinaryModule* GetBinaryModuleVoxels()
{
    static NativeBinaryModule module("Voxels", MAssemblyOptions());
    return &module;
}

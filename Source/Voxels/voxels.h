
#pragma once

#include "Engine/Scripting/Script.h"
#include <Engine/Level/Actors/Camera.h>
#include "Engine/Serialization/Serialization.h"
#include "Engine/Core/Log.h"
#include "Meshing/Volume.h"

API_CLASS(Attributes = "ExecuteInEditMode") class VOXELS_API Voxels : public Script
{
//API_AUTO_SERIALIZATION();
DECLARE_SCRIPTING_TYPE(Voxels);
    void OnEnable() override;
    void OnAwake() override;
    void OnDisable() override;
    void OnDebugDraw() override;
    void Serialize(SerializeStream& stream, const void* otherObj) override;
    void Deserialize(DeserializeStream& stream, ISerializeModifier* modifier) override;
private:
    Camera* observerCamera;
    Volume* volume;
    Array<int32> lodDistances;
public:
    API_PROPERTY(Attributes = "EditorOrder(10), EditorDisplay(\"Properties\"), Tooltip(\"Observer camera.\")")
    void SetObserverCamera(Camera* value);

    API_PROPERTY()
    FORCE_INLINE Camera* GetObserverCamera() const
    {
        return observerCamera;
    }

    API_PROPERTY(Attributes = "EditorOrder(10), EditorDisplay(\"Properties\"), Tooltip(\"LOD distances.\")")
    void SetLodDistances(Array<int32> value);

    API_PROPERTY()
    FORCE_INLINE Array<int32> GetLodDistances() const
    {
        return lodDistances;
    }
};


#include "voxels.h"


Voxels::Voxels(const SpawnParams& params)
    : Script(params)
{
    
}

void Voxels::SetObserverCamera(Camera* value) 
{
    observerCamera = value;
}

void Voxels::SetLodDistances(Array<int32> value)
{
    lodDistances = value;
}

void Voxels::OnEnable()
{
    if (Level::Scenes.Count() > 0) {
        volume = new Volume();
        Scene* scene = Level::Scenes[0];
        Camera* c = GetObserverCamera();
        Actor* a = GetActor();

        volume->Init(VolumeConfig{ Int3(256, 256, 256), 16, 512, c, a, GetLodDistances() }, MesherType::MC);
        volume->FromSDF();
    }
    else {
        // TODO
    }
}

void Voxels::OnAwake()
{
}

void Voxels::OnDisable()
{
}

void Voxels::OnDebugDraw()
{
    //LOG(Info, "OnUpdate");
    volume->UpdateObserverCam(GetObserverCamera());
}


void Voxels::Serialize(SerializeStream& stream, const void* otherObj)
{
    // Base
    Script::Serialize(stream, otherObj);
    SERIALIZE_GET_OTHER_OBJ(Voxels);
    SERIALIZE_MEMBER(Camera, observerCamera);
    SERIALIZE_MEMBER(LodDistances, lodDistances);
}

void Voxels::Deserialize(DeserializeStream& stream, ISerializeModifier* modifier)
{
    // Base
    Script::Deserialize(stream, modifier);
    DESERIALIZE_MEMBER(Camera, observerCamera);
    DESERIALIZE_MEMBER(LodDistances, lodDistances);
}
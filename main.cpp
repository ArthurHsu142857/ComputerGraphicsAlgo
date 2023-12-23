#include "ReflectiveShadowMap.h"

int main()
{
    std::string path = "../scene/nanosuit/nanosuit.obj";
    std::unique_ptr<ReflectiveShadowMap> rsm = std::make_unique<ReflectiveShadowMap>(path.c_str());
    
    rsm->Initialize();

    rsm->Run();
    
    rsm->Destroy();
}
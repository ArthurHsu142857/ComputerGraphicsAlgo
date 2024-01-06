#include "ReflectiveShadowMap.h"

int main()
{
    // Run reflective shadow map APP
    std::string path = "../scene/lucy/lucy_3M.obj";
    std::unique_ptr<ReflectiveShadowMap> rsm = std::make_unique<ReflectiveShadowMap>(path.c_str());
    
    rsm->Initialize();
    rsm->Run();
    rsm->Destroy();
}
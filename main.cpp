#include "ReflectiveShadowMap.h"
#include "SphericalHarmonics.h"

int main()
{
    // [TODO] Choose different APP.
    {
        // Run reflective shadow map APP
        std::string path = "../scene/lucy/lucy_3M_scale.obj";
        std::unique_ptr<ReflectiveShadowMap> rsm = std::make_unique<ReflectiveShadowMap>(path.c_str());

        rsm->Initialize(1600, 1200);
        rsm->Run();
        rsm->Destroy();
    }

    {
        // Run Spherical harmonic lighting APP
        std::string path = "../scene/lucy/lucy_3M_scale.obj";
        std::unique_ptr<SphericalHarmonics> sh = std::make_unique<SphericalHarmonics>(path.c_str());
        sh->Initialize(1600, 1200);
        sh->Run();
        sh->Destroy();
    }
}
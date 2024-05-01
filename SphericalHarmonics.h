#pragma once
#include "Application.h"


class SphericalHarmonics : public Application
{
public:
	SphericalHarmonics(const char* objPath);
	~SphericalHarmonics();

private:
	void SetupGL() override;
	void ProcessKeyboardInput(float deltaTime) override;
	void SetupResource() override;
	void FreeResource() override;
	void RenderLoop() override;

	const char* mpFilePath;
	glm::vec3 mMainCameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	static constexpr float smNear = 0.1f;
	static constexpr float smFar = 100.0f;
	glm::mat4 mProjectionMatrix;

	std::unique_ptr<Model> mpSkyboxMesh;

	std::unique_ptr<Shader> mpSkyboxShader;

};
#pragma once
#include "Application.h"

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ReflectiveShadowMap : public Application {
	// Todo : light style class ...
	struct Light {
		Light(glm::vec3 position, glm::vec3 color) : position(position), color(color) {};
		glm::vec3 position;
		glm::vec3 color;
	};

public:
	ReflectiveShadowMap(const char* objPath);
	~ReflectiveShadowMap();

private:
	void SetupGL() override;
	void ProcessKeyboardInput(float deltaTime) override;
	void SetupResource() override;
	void FreeResource() override;
	void CreateRenderBuffers();
	void RenderLoop() override;
	void RenderLightView();
	void StoreLightInfo();
	void RenderCameraView();
	void RenderQuad();

private:
	// Shaders
	std::unique_ptr<Shader> mpLightShader;
	std::unique_ptr<Shader> mpQuadShader;
	std::unique_ptr<Shader> mpCombineShader;

	// Render information
	std::unique_ptr<Model> mpModel;
	std::unique_ptr<Model> mpQuad;
	std::unique_ptr<Light> mpLight;
	const char* mpFilePath;

	// Buffers
	GLuint mReflectiveShadowMapFBO;
	GLuint mDepthMapTexture;
	GLuint mWorldPoseTexture;
	GLuint mNormalMapTexture;
	GLuint mFluxMapTexture;

	glm::vec3 mMainCameraPosition = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 mLucyPosition = glm::vec3(0.0f, 0.0f, -5.0f);
	glm::vec3 mLucyScale = glm::vec3(0.01f, 0.01f, 0.01f);
	// Todo : light style class
	glm::vec3 mLightPosition = glm::vec3(0.0f, 5.0f, 0.0f);

	// Debug
	int mDebugSwitcher = 0;
	GLuint mDebugColorTexture;


};


#pragma once
#include "Application.h"

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <random>

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
	void GenerateRandomNumbers();
	void RenderLoop() override;
	void PrepareLightingResource();
	void RenderLightView();
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
	GLuint mRandomNumberBuffer;
	GLuint mDepthMapTexture;
	GLuint mWorldPoseTexture;
	GLuint mNormalMapTexture;
	GLuint mFluxMapTexture;

	static const int smSampleNumber = 200;
	static constexpr float smNear = 0.1f;
	static constexpr float smFar = 100.0f;
	glm::vec3 mMainCameraPosition = glm::vec3(0.0f, 5.0f, 0.0f);
	glm::vec3 mLucyPosition = glm::vec3(0.0f, 0.0f, -10.0f);
	glm::vec3 mLucyScale = glm::vec3(1.0f, 1.0f, 1.0f);
	// Todo : light style class
	glm::vec3 mLightPosition = glm::vec3(7.0f, 15.0f, 5.0f);
	glm::mat4 mLucyTramsformMatrix;
	glm::mat4 mProjectionMatrix;
	float mRotateModel = 0.0f;

	GLfloat mRandomNumbers[smSampleNumber * 2];

	// Debug
	int mDebugSwitcher = 0;
	GLuint mDebugColorTexture;
};


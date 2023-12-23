#pragma once
#include "Application.h"

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);
static GLFWwindow* gpWindow;
static Camera* gpMainCamera;


class ReflectiveShadowMap : public Application {
public:
	ReflectiveShadowMap(const char* objPath);
	~ReflectiveShadowMap();

	void Initialize() override;
	void Run() override;
	void Destroy() override;

	void Render() override;

private:
	void SetupGL() override;
	void ProcessInput(float deltaTime);
	void SetupResource();
	void FreeResource();

private:
	std::unique_ptr<Shader> mpShader;
	std::unique_ptr<Model> mpModel;

	const char* mpFilePath;


};


#pragma once
#include "Application.h"

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ReflectiveShadowMap : public Application {
public:
	ReflectiveShadowMap(const char* objPath);
	~ReflectiveShadowMap();

	void Run() override;
	void Render() override;

private:
	void SetupGL() override;
	void ProcessKeyboardInput(float deltaTime) override;
	void SetupResource() override;
	void FreeResource() override;

private:
	std::unique_ptr<Shader> mpShader;
	std::unique_ptr<Model> mpModel;

	const char* mpFilePath;


};


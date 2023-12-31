#pragma once
#include "Application.h"

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ReflectiveShadowMap : public Application {
	// Todo : light style class ...
	struct Light {
		Light(glm::vec3 p, glm::vec3 c) : position(p), color(c) {};
		glm::vec3 position;
		glm::vec3 color;
	};

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
	std::unique_ptr<Light> mpLight;

	const char* mpFilePath;


};


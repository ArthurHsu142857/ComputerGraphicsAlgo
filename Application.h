#pragma once

// include GLAD before GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Model.h"
#include "Camera.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

static const GLsizei WINDOW_WIDTH = 1600;
static const GLsizei WINDOW_HEIGHT = 1200;

class Application {
public:
	void Initialize() {
		SetupGL();
		SetupResource();
	}

	void Destroy() {
		FreeResource();
	}

	virtual void Run() = 0;

protected:
	virtual void SetupGL() = 0;
	virtual void ProcessKeyboardInput(float deltaTime) = 0;
	virtual void SetupResource() = 0;
	virtual void FreeResource() = 0;
	virtual void Render() = 0;	
};

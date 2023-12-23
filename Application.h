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
	Application();
	~Application();

	virtual void Initialize();
	virtual void Run();
	virtual void Destroy();

protected:
	virtual void SetupGL();
	virtual void ProcessInput(float deltaTime);
	virtual void SetupResource();
	virtual void FreeResource();

	virtual void Render();	
};

#pragma once

// include GLAD before GLFW
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "Camera.h"
#include "Model.h"

static GLFWwindow* gpWindow;

static bool gFirstMouseInput = true;
static double gLastX = 0;
static double gLastY = 0;

class Application {
 public:
  void Initialize(GLsizei windowWidth, GLsizei windowHeight) {
    this->mWindowWidth = windowWidth;
    this->mWindowHeight = windowHeight;

    SetupGL();
    SetupResource();
  }

  void Destroy() { FreeResource(); }

  void Run() { RenderLoop(); }

 protected:
  virtual void SetupGL() = 0;
  virtual void ProcessKeyboardInput(float deltaTime) = 0;
  virtual void SetupResource() = 0;
  virtual void FreeResource() = 0;
  virtual void RenderLoop() = 0;

  GLsizei mWindowWidth;
  GLsizei mWindowHeight;
};

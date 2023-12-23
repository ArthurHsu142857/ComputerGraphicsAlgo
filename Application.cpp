#include "Application.h"

#include <iostream>


Application::Application() {

}

Application::~Application() {

}

void Application::Initialize() {
}

void Application::Run() {
}

void Application::Destroy() {
}

void Application::SetupGL() {
}

void Application::ProcessInput(float deltaTime) {
	// Implement by algorithm instance
}

void Application::SetupResource() {
	// Implement by algorithm instance
}

void Application::FreeResource() {
	// Implement by algorithm instance
}

void Application::Render() {
	// Implement by algorithm instance
	/*while (!glfwWindowShouldClose(gWindow)) {
		ProcessInput();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(gWindow);
		glfwPollEvents();
	}*/
}
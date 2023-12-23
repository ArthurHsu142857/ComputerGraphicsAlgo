#include "ReflectiveShadowMap.h"

#include <iostream>

bool gFirstMouseInput = true;
double gLastX = WINDOW_WIDTH / 2;
double gLastY = WINDOW_HEIGHT / 2;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);


ReflectiveShadowMap::ReflectiveShadowMap(const char* objPath) {
	mpFilePath = objPath;
}

ReflectiveShadowMap::~ReflectiveShadowMap() {
}

void ReflectiveShadowMap::Initialize() {
	SetupGL();

	SetupResource();
}

void ReflectiveShadowMap::Run() {
	Render();
}

void ReflectiveShadowMap::Destroy() {
	FreeResource();

	glfwTerminate();
}

void ReflectiveShadowMap::SetupGL() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gpWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Reflective shadow map", NULL, NULL);
	if (gpWindow == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(gpWindow);
	glfwSetFramebufferSizeCallback(gpWindow, FramebufferSizeCallback);
	glfwSetCursorPosCallback(gpWindow, MouseCallback);

	glfwSetInputMode(gpWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return;
	}
}

void ReflectiveShadowMap::ProcessInput(float deltaTime) {
	if (glfwGetKey(gpWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(gpWindow, true);

	if (glfwGetKey(gpWindow, GLFW_KEY_W) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(gpWindow, GLFW_KEY_S) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(gpWindow, GLFW_KEY_A) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(gpWindow, GLFW_KEY_D) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(RIGHT, deltaTime);
}

void ReflectiveShadowMap::SetupResource() {
	gpMainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	mpModel = std::make_unique<Model>(mpFilePath);
	mpShader = std::make_unique<Shader>("../shader/RSM.vs", "../shader/RSM.fs");
}

void ReflectiveShadowMap::FreeResource() {
	glDeleteProgram(mpShader->ID);
	
	mpShader.release();
	mpModel.release();
}

void ReflectiveShadowMap::Render() {
	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(gpWindow)) {
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		ProcessInput(deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mpShader->use();

		glm::mat4 projection = glm::perspective(glm::radians(gpMainCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = gpMainCamera->GetViewMatrix();
		mpShader->setMat4("projection", projection);
		mpShader->setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		mpShader->setMat4("model", model);
		mpModel->Draw(mpShader->ID);

		glfwSwapBuffers(gpWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn) {
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (gFirstMouseInput) {
		gLastX = xPos;
		gLastY = yPos;
		gFirstMouseInput = false;
	}

	float xoffset = xPos - gLastX;
	float yoffset = gLastY - yPos; // reversed since y-coordinates go from bottom to top

	gLastX = xPos;
	gLastY = yPos;

	gpMainCamera->ProcessMouseMovement(xoffset, yoffset);
}

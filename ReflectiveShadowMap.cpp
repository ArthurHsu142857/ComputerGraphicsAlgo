#include "ReflectiveShadowMap.h"

#include <iostream>

bool gFirstMouseInput = true;
double gLastX = WINDOW_WIDTH / 2;
double gLastY = WINDOW_HEIGHT / 2;

static GLFWwindow* gpWindow;
static Camera* gpMainCamera;

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);


ReflectiveShadowMap::ReflectiveShadowMap(const char* objPath) {
	mpFilePath = objPath;
}

ReflectiveShadowMap::~ReflectiveShadowMap() {
}

void ReflectiveShadowMap::Run() {
	Render();
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

void ReflectiveShadowMap::ProcessKeyboardInput(float deltaTime) {
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
	if (glfwGetKey(gpWindow, GLFW_KEY_Q) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(UP, deltaTime);
	if (glfwGetKey(gpWindow, GLFW_KEY_E) == GLFW_PRESS)
		gpMainCamera->ProcessKeyboard(DOWN, deltaTime);
}

void ReflectiveShadowMap::SetupResource() {
	gpMainCamera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

	mpModel = std::make_unique<Model>(mpFilePath);
	mpShader = std::make_unique<Shader>("../shader/RSM.vs", "../shader/RSM.fs");
	mpLight = std::make_unique<Light>(
		glm::vec3(0.0f, 3.0f, 3.0f),
		glm::vec3(1.0f, 1.0f, 1.0f)
	);
}

void ReflectiveShadowMap::FreeResource() {
	glDeleteProgram(mpShader->ID);

	mpShader.release();
	mpModel.release();
}

void ReflectiveShadowMap::Render() {
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(gpWindow)) {
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		ProcessKeyboardInput(deltaTime);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mpShader->use();

		// Set matrix
		glm::mat4 projection = glm::perspective(glm::radians(gpMainCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = gpMainCamera->GetViewMatrix();
		mpShader->setMat4("projection", projection);
		mpShader->setMat4("view", view);

		// Set light position
		mpShader->setVec3("light.position", mpLight->position);
		mpShader->setVec3("light.color", mpLight->color);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -15.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));	// it's a bit too big for our scene, so scale it down
		mpShader->setMat4("model", model);
		mpModel->Draw(mpShader.get());

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

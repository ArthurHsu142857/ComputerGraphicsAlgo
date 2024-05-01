#include "SphericalHarmonics.h"


static Camera* gpMainCamera;

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
static void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn);

SphericalHarmonics::SphericalHarmonics(const char* objPath)
{
	mpFilePath = objPath;
}

SphericalHarmonics::~SphericalHarmonics()
{
}

void SphericalHarmonics::SetupGL()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gpWindow = glfwCreateWindow(this->mWindowWidth, this->mWindowHeight, "Computer graphics algorithm", NULL, NULL);
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

void SphericalHarmonics::ProcessKeyboardInput(float deltaTime)
{
	if (glfwGetKey(gpWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(gpWindow, true);

	// Camera position control
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

void SphericalHarmonics::SetupResource()
{
	gpMainCamera = new Camera(mMainCameraPosition);
	
	mpSkyboxMesh = std::make_unique<Model>(Model::SKY_BOX, "../resources/skybox");

	mpSkyboxShader = std::make_unique<Shader>("../shader/Skybox.vs", "../shader/Skybox.fs");

	mProjectionMatrix = glm::perspective(glm::radians(gpMainCamera->Zoom), (float)mWindowWidth / (float)mWindowHeight, smNear, smFar);

}

void SphericalHarmonics::FreeResource()
{
	mpFilePath = nullptr;
}

void SphericalHarmonics::RenderLoop()
{
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, mWindowWidth, mWindowHeight);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(gpWindow))
	{
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mpSkyboxShader->use();

		glm::mat4 viewMatrix = glm::mat4(glm::mat3(gpMainCamera->GetViewMatrix())); // remove translation from the view matrix
		mpSkyboxShader->setMat4("view", viewMatrix);
		mpSkyboxShader->setMat4("projection", mProjectionMatrix);

		mpSkyboxMesh->Draw(mpSkyboxShader.get());

		ProcessKeyboardInput(deltaTime);

		glfwSwapBuffers(gpWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void MouseCallback(GLFWwindow* window, double xPosIn, double yPosIn)
{
	float xPos = static_cast<float>(xPosIn);
	float yPos = static_cast<float>(yPosIn);

	if (gFirstMouseInput)
	{
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
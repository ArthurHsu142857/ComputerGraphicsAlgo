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

	// Debug switcher
	if (glfwGetKey(gpWindow, GLFW_KEY_1) == GLFW_PRESS)
		mDebugSwitcher = 1;
	if (glfwGetKey(gpWindow, GLFW_KEY_2) == GLFW_PRESS)
		mDebugSwitcher = 2;
	if (glfwGetKey(gpWindow, GLFW_KEY_3) == GLFW_PRESS)
		mDebugSwitcher = 3;
	if (glfwGetKey(gpWindow, GLFW_KEY_4) == GLFW_PRESS)
		mDebugSwitcher = 4;
	if (glfwGetKey(gpWindow, GLFW_KEY_0) == GLFW_PRESS)
		mDebugSwitcher = 0;
}

void ReflectiveShadowMap::SetupResource() {
	gpMainCamera = new Camera(mMainCameraPosition);

	mpModel = std::make_unique<Model>(Model::LOAD_MODEL, mpFilePath);
	mpQuad = std::make_unique<Model>(Model::FULL_SCREEN_QUAD);
	mpLightShader = std::make_unique<Shader>("../shader/RSM.vs", "../shader/RSM.fs");
	mpQuadShader = std::make_unique<Shader>("../shader/FullScreenQuad.vs", "../shader/FullScreenQuad.fs");
	mpLight = std::make_unique<Light>(
		mLightPosition,
		glm::vec3(1.0f, 1.0f, 1.0f)
	);

	CreateRenderBuffers();
}

void ReflectiveShadowMap::FreeResource() {
	glDeleteProgram(mpLightShader->ID);
	glDeleteProgram(mpQuadShader->ID);

	glDeleteTextures(1, &mDepthMapTexture);
	glDeleteTextures(1, &mWorldPoseTexture);
	glDeleteTextures(1, &mNormalMapTexture);
	glDeleteTextures(1, &mFluxMapTexture);
	glDeleteTextures(1, &mDebugColorTexture);
	
	glDeleteFramebuffers(1, &mReflectiveShadowMapFBO);
}

void ReflectiveShadowMap::CreateRenderBuffers() {
	glGenFramebuffers(1, &mReflectiveShadowMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mReflectiveShadowMapFBO);
	
	// World position texture
	glGenTextures(1, &mWorldPoseTexture);
	glBindTexture(GL_TEXTURE_2D, mWorldPoseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mWorldPoseTexture, 0);

	// Normal map texture
	glGenTextures(1, &mNormalMapTexture);
	glBindTexture(GL_TEXTURE_2D, mNormalMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, mNormalMapTexture, 0);

	// Flux map texture
	glGenTextures(1, &mFluxMapTexture);
	glBindTexture(GL_TEXTURE_2D, mFluxMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, mFluxMapTexture, 0);

	// Todo : use render buffer object ?
	// Depth map texture
	glGenTextures(1, &mDepthMapTexture);
	glBindTexture(GL_TEXTURE_2D, mDepthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthMapTexture, 0);
	
	// Debug color texture
	glGenTextures(1, &mDebugColorTexture);
	glBindTexture(GL_TEXTURE_2D, mDebugColorTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, mDebugColorTexture, 0);

	GLuint attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4, attachments);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReflectiveShadowMap::RenderLoop() {
	glEnable(GL_CULL_FACE);

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	float deltaTime = 0.0f;
	float lastFrameTime = 0.0f;

	while (!glfwWindowShouldClose(gpWindow)) {
		float currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;

		ProcessKeyboardInput(deltaTime);

		// Get light position view information
		RenderCameraView();

		// Present light information
		RenderQuad();

		glfwSwapBuffers(gpWindow);
		glfwPollEvents();
	}

	glfwTerminate();
}

void ReflectiveShadowMap::RenderCameraView() {
	// Render to texture
	glBindFramebuffer(GL_FRAMEBUFFER, mReflectiveShadowMapFBO);

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mpLightShader->use();

	// Set transform matrix
	glm::mat4 projection = glm::perspective(glm::radians(gpMainCamera->Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	glm::vec3 viewDir = mLucyPosition - mLightPosition;
	glm::mat4 view = glm::lookAt(mLightPosition, mLightPosition + viewDir, glm::vec3(0.0f, 1.0f, 0.0f));
	mpLightShader->setMat4("projection", projection);
	mpLightShader->setMat4("view", view);
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, mLucyPosition);
	model = glm::scale(model, mLucyScale);
	mpLightShader->setMat4("model", model);

	// Set light information
	mpLightShader->setVec3("light.position", mpLight->position);
	mpLightShader->setVec3("light.color", mpLight->color);

	// Render
	mpModel->Draw(mpLightShader.get());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ReflectiveShadowMap::RenderQuad() {
	// Render gbuffer texture
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glDisable(GL_DEPTH_TEST);

	mpQuadShader->use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mWorldPoseTexture);
	mpQuadShader->setInt("worldPoseTexture", 0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mNormalMapTexture);
	mpQuadShader->setInt("normalMapTexture", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, mFluxMapTexture);
	mpQuadShader->setInt("fluxMapTexture", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, mDepthMapTexture);
	mpQuadShader->setInt("depthMapTexture", 3);
	
	// Debug color texture
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, mDebugColorTexture);
	mpQuadShader->setInt("debugColorTexture", 4);

	mpQuadShader->setInt("debugSwitchTexture", mDebugSwitcher);

	mpQuad->Draw(mpQuadShader.get());
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

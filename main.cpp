#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "camera.h"
#include "resource_manager.h"
#include "renderer.h"
#include "game_object.h"

#include <iostream>

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 1280;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1024;

Game SpaceRam(SCREEN_WIDTH, SCREEN_HEIGHT);
// BG color
const glm::vec4 BG_COLOR(0.1f, 0.1f, 0.15f, 1.0f);

int main(int argc, char *argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "SpaceRam", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError();

	glfwSetKeyCallback(window, key_callback);

	// OpenGL configuration
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_MULTISAMPLE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);

	// Initialize game
	SpaceRam.Init();
	// Camera Init
	Camera GameCamera(glm::vec3((24/2)-0.5f, 21.0f, 11.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, -40.0f);
	glm::mat4 projection = glm::perspective(GameCamera.Zoom, (GLfloat)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);

	// Test cube
	std::vector<GLfloat> vertices {
		 // Position			//Normal
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,	0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,	0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	-1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,	0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,	0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,	0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,	0.0f,  1.0f,  0.0f
	};

	LightSource *lightSource = new LightSource{
		glm::vec3((24/2)-0.5f, 15.0f, 2.0f),
		glm::vec3(0.8f, 0.5f, 1.0f),
	};
	
	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");
	Shader outlineCube = ResourceManager::LoadShader("outlineCube", "shaders/outline.vs", "shaders/outline.frag", "shaders/outline.gs");

	Renderer cubeRenderer(testCube, vertices);
	Renderer outlineRenderer(outlineCube, vertices);

	testCube.Use().SetMatrix4("projection", projection);
	outlineCube.Use().SetMatrix4("projection", projection);

	// Will be replaced by "GameLevel" object
	int levelWidth = 24;
	int numCubes = levelWidth * 30;
	std::vector<GameObject> cubes;
	for (int i = 0, j = 0; i < numCubes; i++) {
		glm::vec3 color(0.31f, 1.0f, 0.31f);
		if (i >= numCubes / 2) color = glm::vec3(0.31f, 0.31f, 1.0f);
		GLfloat x = 1.0f * (i%levelWidth);
		if (i != 0 && i % levelWidth == 0) {
			j++;
		}
		GLfloat z = -1.0f * j;
		cubes.push_back(GameObject(glm::vec3(x, 0.0f, z), glm::vec3(), color, 0.0f));	
	}

	// Start Game within Menu State
	SpaceRam.State = GameState::GAME_ACTIVE;

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Camera
	glm::mat4 view = GameCamera.GetViewMatrix();
	while (!glfwWindowShouldClose(window)) {
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		//deltaTime = 0.001f;
		// Manage user input
		SpaceRam.ProcessInput(deltaTime);

		// Update Game state
		SpaceRam.Update(deltaTime);

		// Render
		glClearColor(BG_COLOR.x, BG_COLOR.y, BG_COLOR.z, BG_COLOR.w);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SpaceRam.Render();

		for (auto &cube : cubes) {
			cube.Draw(cubeRenderer, view, lightSource);
			cube.Draw(outlineRenderer, view, lightSource);
		}

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();

	delete lightSource;
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	// When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			SpaceRam.Keys[key] = GL_TRUE;
		}
		else if (action == GLFW_RELEASE) {
			SpaceRam.Keys[key] = GL_FALSE;
		}
	}
}

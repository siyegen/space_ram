#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "resource_manager.h"

// GLFW function declerations
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// The Width of the screen
const GLuint SCREEN_WIDTH = 1280;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1024;

Game SpaceRam(SCREEN_WIDTH, SCREEN_HEIGHT);

int main(int argc, char *argv[]) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

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

	// Test cube
	GLfloat vertices[] = {
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
	
	Shader testCube = ResourceManager::LoadShader("testCube", "shaders/simple3d.vs", "shaders/diffuse_only.frag");

	//GLuint VBO, testCubeVAO;
	//glGenVertexArrays(1, &testCubeVAO);
	//glGenBuffers(1, &VBO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//glBindVertexArray(testCubeVAO);
	//// Position attribute
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	//glEnableVertexAttribArray(0);
	//// Normal attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	//glEnableVertexAttribArray(1);
	//glBindVertexArray(0); // Unbind VAO

	//// Test Camera Setup
	//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)(SCREEN_WIDTH / SCREEN_HEIGHT), 0.1f, 100.0f);

	// Start Game within Menu State
	SpaceRam.State = GameState::GAME_ACTIVE;

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
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
		glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		SpaceRam.Render();

		glfwSwapBuffers(window);
	}

	// Delete all resources as loaded using the resource manager
	ResourceManager::Clear();

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
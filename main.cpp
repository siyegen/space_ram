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
#include "game_level.h"

#include <iostream>


// GLFW function declerations
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mousekey_callback(GLFWwindow *window, int button, int action, int mods);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);

// The Width of the screen
const GLuint SCREEN_WIDTH = 1280;
// The height of the screen
const GLuint SCREEN_HEIGHT = 1024;

Game SpaceRam(SCREEN_WIDTH, SCREEN_HEIGHT);
// BG color
const glm::vec4 BG_COLOR(0.1f, 0.1f, 0.15f, 1.0f);

int main() {
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
	glfwSetMouseButtonCallback(window, mousekey_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

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

	// Start Game within Menu State
	SpaceRam.State = GameState::MENU;

	// DeltaTime variables
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;

	// Main loop here
	while (!glfwWindowShouldClose(window)) {
		// Calculate delta time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		// Manage user input, this should swap scene/states as needed
		SpaceRam.ProcessInput(deltaTime);

		// Update Game state/scene
		SpaceRam.Update(deltaTime);

		// Render, this should be calling the renederer from the scene manager
		glClearColor(BG_COLOR.x, BG_COLOR.y, BG_COLOR.z, BG_COLOR.w);
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

void mousekey_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		GLdouble xPos;
		GLdouble yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		if (xPos && yPos) {
			SpaceRam.HandleClick(GLFW_MOUSE_BUTTON_LEFT, xPos, yPos);
		}
		SpaceRam.MouseHeld = true;
	} else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
		SpaceRam.MouseHeld = false;
	}
}

void mouse_callback(GLFWwindow *window, double xpos, double ypos) {
	if (SpaceRam.State == GameState::ACTIVE) {
		SpaceRam.MoveCursor(xpos, ypos);
	}
}

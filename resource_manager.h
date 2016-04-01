#pragma once

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <GL/glew.h>

#include "shader.h"


// Singleton ResourceManager used to load a shader from file
class ResourceManager final {
public:
	// Resource storage
	static std::map<std::string, Shader> Shaders;

	// Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
	static Shader LoadShader(std::string name, const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile=nullptr);

	// Retrieves a stored sader
	static Shader GetShader(std::string name);

	// Properly de-allocates all loaded resources
	static void Clear();

private:
	// Private constructor to avoid init
	ResourceManager() {}

	// Loads and generates a shader from file
	static Shader loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile = nullptr);
	static std::string readFromFile(const GLchar* shaderFile);
};

#endif
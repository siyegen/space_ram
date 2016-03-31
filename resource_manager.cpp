#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Shader> ResourceManager::Shaders;

Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile, std::string name) {
	Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
	return Shaders[name];
}

Shader ResourceManager::GetShader(std::string name) {
	return Shaders[name];
}

void ResourceManager::Clear() {
	// (Properly) delete all shaders	
	for (auto iter : Shaders)
		glDeleteProgram(iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const GLchar* vShaderFile, const GLchar* fShaderFile, const GLchar* gShaderFile) {
	const GLchar *vShaderCode = nullptr;
	const GLchar *fShaderCode = nullptr;
	const GLchar *gShaderCode = nullptr;

	try {
		vShaderCode = readFromFile(vShaderFile);
		fShaderCode = readFromFile(fShaderFile);
		// Skip geometry shader if no file is given
		if (gShaderFile) {
			gShaderCode = readFromFile(gShaderFile);
		}
	} catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderCode);
	delete vShaderCode, fShaderCode, gShaderCode;
	return shader;
}

GLchar* ResourceManager::readFromFile(const GLchar *sFile) {
	std::string shaderCode;
	std::stringstream shaderStream;
	std::ifstream shaderFile(sFile);

	shaderStream << shaderFile.rdbuf();
	shaderFile.close();

	return (GLchar*)shaderStream.str().c_str();
}

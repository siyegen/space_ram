#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>

std::map<std::string, Shader> ResourceManager::Shaders;
Shader ResourceManager::LoadShader(std::string name, const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile) {
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
	std::string vertexCode, fragmentCode, geometryCode;
	try {
		vertexCode = readFromFile(vShaderFile);
		fragmentCode = readFromFile(fShaderFile);
		// Skip geometry shader if no file is given
		if (gShaderFile) {
			geometryCode = readFromFile(gShaderFile);
		}
	} catch (std::exception e) {
		std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
	}
	const GLchar *vShaderCode = vertexCode.c_str();
	const GLchar *fShaderCode = fragmentCode.c_str();
	const GLchar *gShaderCode = nullptr;
	if (gShaderFile) gShaderCode = geometryCode.c_str();

	Shader shader;
	shader.Compile(vShaderCode, fShaderCode, gShaderCode);
	return shader;
}

std::string ResourceManager::readFromFile(const GLchar *sFile) {
	std::ifstream shaderFile;
	std::stringstream shaderStream;
	shaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

	shaderFile.open(sFile);
	shaderStream << shaderFile.rdbuf();
	shaderFile.close();
	return shaderStream.str();
}

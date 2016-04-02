#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <map>

enum class GLCOMPILETYPE {
	VERTEX,
	FRAGMENT,
	GEOMETRY,
	PROGRAM
};


// [] isn't defined for a const map, can use .find() instead or wrap in function
const std::map<GLCOMPILETYPE, std::string> GLCOMPILETYPESTRMAP = {
	{ GLCOMPILETYPE::VERTEX, "VERTEX" },
	{ GLCOMPILETYPE::FRAGMENT, "FRAGMENT" },
	{ GLCOMPILETYPE::GEOMETRY, "GEOMETRY" },
	{ GLCOMPILETYPE::PROGRAM, "PROGRAM" },
};

// Simple shader class from http://www.learnopengl.com/ with a few tweaks
class Shader {
public:
	// State
	GLuint ID;
	// Constructor
	Shader() {}

	// Sets the current shader as active, do we need to return?
	Shader& Use();

	// Not sure compile should be it's own step separate from constructor
	void Compile(const GLchar *vertexSource, const GLchar *fragmentSource, const GLchar *geometrySource = nullptr);

	void SetFloat(const GLchar *name, GLfloat value, GLboolean useShader = false);
	void SetInteger(const GLchar *name, GLint value, GLboolean useShader = false);

	void SetVector2f(const GLchar *name, GLfloat x, GLfloat y, GLboolean useShader = false);
	void SetVector2f(const GLchar *name, const glm::vec2 &value, GLboolean useShader = false);

	void SetVector3f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = false);
	void SetVector3f(const GLchar *name, const glm::vec3 &value, GLboolean useShader = false);

	void SetVector4f(const GLchar *name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = false);
	void SetVector4f(const GLchar *name, const glm::vec4 &value, GLboolean useShader = false);

	void SetMatrix4(const GLchar *name, const glm::mat4 &matrix, GLboolean useShader = false);
private:
	// Checks if compilation or linking failed and if so, print the error logs
	void checkCompileErrors(const GLuint &object, GLCOMPILETYPE type);
	static std::string GLCOMPILETYPE_to_string(const GLCOMPILETYPE type);
};

#endif
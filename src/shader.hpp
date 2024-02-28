#pragma once

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

enum ShaderFailureType {
	Io,
	Compile,
	Link,
};

class ShaderFailure : public std::exception {
public:
	ShaderFailureType t;

	ShaderFailure(ShaderFailureType sft);

	const char *what() const noexcept;
};

class Shader {
public:
	GLuint id;

	Shader(const char *path, GLenum shaderType);
};

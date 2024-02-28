#pragma once

#include <shader.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShaderProgram {
public:
	GLuint id;

	ShaderProgram(Shader fragmentShader, Shader vertexShader);

	void setFloat(const char *name, float value) const;
	void setInt(const char *name, int value) const;
	void setMat4(const char *name, const glm::mat4& mat);
};

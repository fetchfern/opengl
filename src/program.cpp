#include <program.hpp>
#include <shader.hpp>
#include <glm/gtc/type_ptr.hpp>

ShaderProgram::ShaderProgram(Shader fragmentShader, Shader vertexShader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, fragmentShader.id);
	glAttachShader(program, vertexShader.id);
	glLinkProgram(program);

	int ok;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);

	if (!ok) {
		char log[1024];
		glGetProgramInfoLog(program, sizeof(log)/sizeof(char), NULL, log);
		std::cout << "shader linkage error dump ---------------\n" << log << "\n"
				  << "dump end --------------------------------";
		throw ShaderFailure(ShaderFailureType::Link);
	}

	glDeleteShader(fragmentShader.id);
	glDeleteShader(vertexShader.id);

	this->id = program;
};

void ShaderProgram::setFloat(const char *name, float value) const {
	glUniform1f(glGetUniformLocation(this->id, name), value);
}

void ShaderProgram::setInt(const char *name, int value) const {
	glUniform1i(glGetUniformLocation(this->id, name), value);
}

void ShaderProgram::setMat4(const char *name, const glm::mat4& mat) {
	// location, send one matrix, do not transpose the matrix, the matrix as const float*
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, glm::value_ptr(mat));
}

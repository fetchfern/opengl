#include <shader.hpp>

ShaderFailure::ShaderFailure(ShaderFailureType sft) {
	this->t = sft;
}

const char *ShaderFailure::what() const noexcept {
	switch (this->t) {
		case ShaderFailureType::Io : return (char *)"Failed to open file";
		case ShaderFailureType::Compile : return (char *)"Failed to compile shader";
		case ShaderFailureType::Link : return (char *)"Failed to link shaders";
		default: return NULL;
	}
}

Shader::Shader(const char *path, GLenum shaderType) {
	std::ifstream file;
	std::string code;

    file.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try {
		file.open(path);

		std::stringstream stream;
		stream << file.rdbuf();

		file.close();
		code = stream.str();
	} catch (std::ifstream::failure &e) {
		throw ShaderFailure(ShaderFailureType::Io);
	}

	GLuint shader = glCreateShader(shaderType);
	const GLchar *codeCstr = code.c_str();
	glShaderSource(shader, 1, &codeCstr, NULL);
	glCompileShader(shader);

	int ok;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);

	if (!ok) {
		char log[1024];
		glGetShaderInfoLog(shader, sizeof(log)/sizeof(char), NULL, log);
		std::cout << "shader compile failure dump ----------------\n" << log << "\n"
			      << "end dump -----------------------------------\n";
		throw ShaderFailure(ShaderFailureType::Compile);
	}

	this->id = shader;
}

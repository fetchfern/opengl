#include "glm/gtc/constants.hpp"
#include <iostream>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <shader.hpp>
#include <program.hpp>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebufferSizeCallback(GLFWwindow *_window, int w, int h) {
	glViewport(0, 0, w, h);
}

void processInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	static GLboolean wireframe = GL_FALSE;
	static GLboolean released = GL_TRUE;

	int w = glfwGetKey(window, GLFW_KEY_O);
	if (w) {
		if (released) {
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}
		released = GL_FALSE;
	} else {
		released = GL_TRUE;
	}
}

int loadTextures(GLuint textures[2]) {
	glGenTextures(2, textures);
	stbi_set_flip_vertically_on_load(true);

	int width, height, nbrChannels;
	unsigned char *data = stbi_load("assets/wall.jpg", &width, &height, &nbrChannels, 0);

	if (!data) {
		std::cerr << "no wall\n";
		return 1;
	}

	glBindTexture(GL_TEXTURE_2D, textures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	data = stbi_load("assets/creature.jpg", &width, &height, &nbrChannels, 0);

	if (!data) {
		std::cerr << "no creature\n";
		return 2;
	}

	glBindTexture(GL_TEXTURE_2D, textures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);

	return 0;
}

GLFWwindow *windowInit() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(800, 600, "an OpenGL window", NULL, NULL);

	if (!window) {
		return NULL;
	}

	glfwMakeContextCurrent(window);
	
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  

	return window;
}

int main() {
	GLFWwindow *window = windowInit();

	if (!window) {
		std::cerr << "Window initialization failed\n";
		return -1;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Couldn't initialize GLAD\n";
		return -2;
	}

	glEnable(GL_DEPTH_TEST);

	int framebufW, framebufH;
	glfwGetFramebufferSize(window, &framebufW, &framebufH);
	glViewport(0, 0, framebufW, framebufH);

	Shader fragment("shaders/shader.frag", GL_FRAGMENT_SHADER);
	Shader vertex("shaders/shader.vert", GL_VERTEX_SHADER);
	ShaderProgram program(fragment, vertex);

	GLuint textures[2];
	int ok = loadTextures(textures);

	std::cout << ok << "\n";

	if (ok != 0) {
		std::cerr << "Failed to load textures\n";
		return -3;
	}

	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glm::vec3 positions[] = {
    	glm::vec3( 0.0f,  0.0f,  -2.0f),
    	glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

	GLuint vbo, vao;
	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);

	/*
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	*/

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint stride = 5 * sizeof(float);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void *)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glUseProgram(program.id);

	program.setInt("texture0", 0 /* GL_TEXTURE0 */);
	program.setInt("texture1", 1 /* GL_TEXTURE1 */);

	glUseProgram((GLuint)0);

	double lastTime = 0;

	glm::vec3 cameraPosition(-0.2f, 0.f, 7.0f);
	glm::vec3 cameraFront(0.f, 0.f, -1.f);
	glm::vec3 cameraUp(0.f, 1.f, 0.f);

	double pitch = 0.L;
	double yaw = 0.L;

	double xcurInit, ycurInit;
	glfwGetCursorPos(window, &xcurInit, &ycurInit);
	glm::vec2 lastCursor(xcurInit, ycurInit);

	// MAIN LOOP
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(64.f / 256.f, 54.f / 256.f, 34.f / 256.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		glUseProgram(program.id);

		double time = glfwGetTime();
		double delta = time - lastTime;
		lastTime = time;

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		// constants
		const glm::mat4 identity(1.f);

		GLfloat fov = glm::radians(45.f);
		GLfloat aspectRatio = (float)width/(float)height;
		GLfloat nearPlane = 0.1f;
		GLfloat farPlane = 100.f;

		double xcur, ycur;
		glfwGetCursorPos(window, &xcur, &ycur);

		double xdelta = xcur - lastCursor.x;
		double ydelta = ycur - lastCursor.y;

		lastCursor.x = xcur;
		lastCursor.y = ycur;

		double sensitivity = 0.0005L;
		pitch -= ydelta * sensitivity;
		yaw += xdelta * sensitivity;

		const double pitchEpsilon = 1.5L / 10000.0L;
		const double rightAngle = glm::tau<double>() / 4.L;

		if (pitch > rightAngle - pitchEpsilon) {
			pitch = rightAngle - pitchEpsilon;
		} else if (pitch < -rightAngle + pitchEpsilon) {
			pitch = -rightAngle + pitchEpsilon;
		}

		pitch = std::remainder(pitch, glm::tau<double>());
		yaw = std::remainder(yaw, glm::tau<double>());

		float x = std::cos(yaw) * std::cos(pitch);
		float y = std::sin(pitch);
		float z = std::sin(yaw) * std::cos(pitch);

		glm::vec3 direction(x, y, z);
		//cameraFront = glm::normalize(direction);

		float cameraSpeed = 5.L * delta;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPosition += cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPosition -= cameraSpeed * cameraFront;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPosition += cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPosition -= cameraSpeed * glm::normalize(glm::cross(cameraFront, cameraUp));
			
		// view matrix/camera
		//glm::vec3 position(0.f, 0.f, 3.f);
		glm::mat4 view = glm::lookAt(
			cameraPosition,
			cameraPosition + cameraFront,
			cameraUp
		);

		// projection matrix
		glm::mat4 projection = glm::perspective(fov, aspectRatio, nearPlane, farPlane);

		program.setMat4("view", view);
		program.setMat4("projection", projection);

		glBindVertexArray(vao);

		for (int i = 0; i < (int)(sizeof(positions)/sizeof(glm::vec3)); i++) {
			float change = (float)time * 0.95f - 4.f;
			glm::mat4 model = glm::translate(identity, positions[i]);
			model = glm::translate(model, glm::vec3(change, change * 1.32f, 0.f));
			model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
			model = glm::rotate(model, glm::radians(22.7f + (float)time) * i, glm::vec3(1.f - (float)i/36.f, 0.5f + (float)i/100.f, 0.1f + (float)i/70.f));
			
			program.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray((GLuint)0);

		//glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	std::cout << "Gracefully exiting...\n";
	glfwTerminate();
	return 0;
}


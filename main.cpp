#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Shader.h"

const unsigned int WIDTH = 640;
const unsigned int HEIGHT = 360;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	std::cout << "Welcome to Margit Game Engine..." << std::endl << std::endl << "An open-source game engine written in OpenGL 4.6. Author is Nathan Baylon." << std::endl;

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Margit Game Engine", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, WIDTH, HEIGHT);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float vertices[] = {
		0.5f, 0.5f, 0.0f,	// Top right
		0.5f, -0.5f, 0.0f,	// Bottom right
		-0.5f, -0.5f, 0.0f,	// Bottom left
		-0.5f, 0.5f, 0.0f,	// Top left
	};

	float triangleOne[] = {
		-0.5f, 0.0f, 0.0f,
		-0.25f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f
	};

	float triangleTwo[] = {
		0.0f, 0.0f, 0.0f,
		0.25f, 0.5f, 0.0f,
		0.5f, 0.0f, 0.0f
	};

	float triangleThree[] = {
		// positions			//colors
		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.25f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.25f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	float triangleFour[] = {
		-1.0f, -1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f
	};

	// Vertex buffer object - generate buffer id, bind it to array buffer and then copy data to the bound buffer
	GLuint VBOs[4];
	glGenBuffers(4, VBOs);

	// Vertex Array Object
	GLuint VAOs[4];
	glGenVertexArrays(4, VAOs);

	// Triangle One
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Specify how the input vertices get interpreted by OpenGL
	glEnableVertexAttribArray(0);

	// Triangle Two
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Triangle Three
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleThree), triangleThree, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[2]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// Triangle Four
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleFour), triangleFour, GL_STATIC_DRAW);

	glBindVertexArray(VAOs[3]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	Shader shaderOne("Shaders/flipTriangle.vert", "Shaders/shader.frag");
	Shader shaderTwo("Shaders/shader.vert", "Shaders/greenBlink.frag");
	Shader shaderThree("Shaders/colorShader.vert", "Shaders/rainbow.frag");
	Shader shaderFour("Shaders/position.vert", "Shaders/position.frag");

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shaderOne.use();

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderTwo.use();
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderTwo.getID(), "ourColor");

		glUniform4f(vertexColorLocation, greenValue, greenValue, greenValue, 1.0f);
		shaderTwo.setFloat("xOffset", 0.5f);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderThree.use();
		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		shaderFour.use();
		glBindVertexArray(VAOs[3]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);

	glDeleteProgram(shaderOne.getID());
	glDeleteProgram(shaderTwo.getID());
	glDeleteProgram(shaderThree.getID());

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
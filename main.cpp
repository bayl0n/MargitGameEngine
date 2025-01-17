#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

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

	unsigned int indices[] = {
		3, 4, 5
	};

	// Vertex buffer object - generate buffer id, bind it to array buffer and then copy data to the bound buffer
	unsigned int VBO, VBO2;
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &VBO2);

	// Vertex Array Object
	unsigned int VAO, VAO2;
	glGenVertexArrays(1, &VAO);
	glGenVertexArrays(1, &VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleOne), triangleOne, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // Specify how the input vertices get interpreted by OpenGL
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleTwo), triangleTwo, GL_STATIC_DRAW);

	glBindVertexArray(VAO2);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Element Buffer Object
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// Vertex shader - create vertex shader, attach the source code to it and then compile. Then check if the compilation was successful
	const char* vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader - create fragment shader, attach the source code. Then check if the compilation was successful
	const char* fragmentShaderSourceRed = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 0.2f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int fragmentShaderRed;
	fragmentShaderRed = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderRed, 1, &fragmentShaderSourceRed, NULL);
	glCompileShader(fragmentShaderRed);

	glGetShaderiv(fragmentShaderRed, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderRed, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Yellow Fragment Shader
	const char* fragmentShaderSourceYellow = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
		"}\0";

	unsigned int fragmentShaderYellow;
	fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderSourceYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	glGetShaderiv(fragmentShaderYellow, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program - create program, attach and link vertex and fragment shaders to it, check compilation, then use program
	unsigned int shaderProgramRed;
	shaderProgramRed = glCreateProgram();

	glAttachShader(shaderProgramRed, vertexShader);
	glAttachShader(shaderProgramRed, fragmentShaderRed);
	glLinkProgram(shaderProgramRed);

	glGetProgramiv(shaderProgramRed, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramRed, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	unsigned int shaderProgramYellow;
	shaderProgramYellow = glCreateProgram();

	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	glGetProgramiv(shaderProgramYellow, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramYellow, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderRed);
	glDeleteShader(fragmentShaderYellow);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramRed);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramYellow);

		glBindVertexArray(VAO2);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);

	glDeleteProgram(shaderProgramRed);
	glDeleteProgram(shaderProgramYellow);

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
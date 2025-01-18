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

	float triangleThree[] = {
		// positions			//colors
		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		-0.25f, -0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
		0.25f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		3, 4, 5
	};

	// Vertex buffer object - generate buffer id, bind it to array buffer and then copy data to the bound buffer
	GLuint VBOs[3];
	glGenBuffers(3, VBOs);

	// Vertex Array Object
	GLuint VAOs[3];
	glGenVertexArrays(3, VAOs);

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

	// Element Buffer Object
	/*unsigned int EBO;
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	// Vertex shader - create vertex shader, attach the source code to it and then compile. Then check if the compilation was successful
	const char* vertexShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"out vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos.xyz, 1.0);\n"
		"	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
		"}\0";

	// Additional color attributes
	const char* vertexColorShaderSource = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"			// the position vairable has attribute position 0
		"layout (location = 1) in vec3 aColor;\n"		// the color variable has attribute position 1
		"out vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"	gl_Position = vec4(aPos, 1.0);\n"
		"	ourColor = aColor;\n"
		"}\0";

	GLuint vertexShader;
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

	GLuint vertexColorShader;
	vertexColorShader = glCreateShader(GL_VERTEX_SHADER);

	glShaderSource(vertexColorShader, 1, &vertexColorShaderSource, NULL);
	glCompileShader(vertexColorShader);

	glGetShaderiv(vertexColorShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexColorShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment shader - create fragment shader, attach the source code. Then check if the compilation was successful
	const char* fragmentShaderSourceOne = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"in vec4 vertexColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vertexColor;\n"
		"}\0";

	GLuint fragmentShaderOne;
	fragmentShaderOne = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderOne, 1, &fragmentShaderSourceOne, NULL);
	glCompileShader(fragmentShaderOne);

	glGetShaderiv(fragmentShaderOne, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderOne, 512, NULL, infoLog);
		std::cout << "ERROR:SHADER::FRAGMENT1::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Second Fragment Shader
	const char* fragmentShaderSourceTwo = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"uniform vec4 ourColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = ourColor;\n"
		"}\0";

	GLuint fragmentShaderTwo;
	fragmentShaderTwo = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderTwo, 1, &fragmentShaderSourceTwo, NULL);
	glCompileShader(fragmentShaderTwo);

	glGetShaderiv(fragmentShaderTwo, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderTwo, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT2::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Third Fragment Shader
	const char* fragmentShaderSourceThree = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"in vec3 ourColor;\n"
		"void main()\n"
		"{\n"
		"	FragColor = vec4(ourColor, 1.0);\n"
		"}\0";

	GLuint fragmentShaderThree;

	fragmentShaderThree = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderThree, 1, &fragmentShaderSourceThree, NULL);
	glCompileShader(fragmentShaderThree);

	glGetShaderiv(fragmentShaderThree, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShaderThree, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT3::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader program - create program, attach and link vertex and fragment shaders to it, check compilation, then use program
	GLuint shaderProgramOne;
	shaderProgramOne = glCreateProgram();

	glAttachShader(shaderProgramOne, vertexShader);
	glAttachShader(shaderProgramOne, fragmentShaderOne);
	glLinkProgram(shaderProgramOne);

	glGetProgramiv(shaderProgramOne, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramOne, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgramTwo;
	shaderProgramTwo = glCreateProgram();

	glAttachShader(shaderProgramTwo, vertexShader);
	glAttachShader(shaderProgramTwo, fragmentShaderTwo);
	glLinkProgram(shaderProgramTwo);

	glGetProgramiv(shaderProgramTwo, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramTwo, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgramThree;
	shaderProgramThree = glCreateProgram();

	glAttachShader(shaderProgramThree, vertexColorShader);
	glAttachShader(shaderProgramThree, fragmentShaderThree);
	glLinkProgram(shaderProgramThree);

	glGetProgramiv(shaderProgramThree, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgramThree, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(vertexColorShader);
	glDeleteShader(fragmentShaderOne);
	glDeleteShader(fragmentShaderTwo);
	glDeleteShader(fragmentShaderThree);

	 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgramOne);

		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgramTwo, "ourColor");

		glUseProgram(shaderProgramTwo);
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		glBindVertexArray(VAOs[1]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glUseProgram(shaderProgramThree);
		glBindVertexArray(VAOs[2]);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(3, VAOs);
	glDeleteBuffers(3, VBOs);

	glDeleteProgram(shaderProgramOne);
	glDeleteProgram(shaderProgramTwo);

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
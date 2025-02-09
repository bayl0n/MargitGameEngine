#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "FastNoiseLite.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <iostream>

#include "Shader.h"
#include "Camera.h"

namespace Margit {
	class Application
	{
	public:
		int run();

		~Application();

	private:
		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
		static void processInput(GLFWwindow* window);

		static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};
}


#include <vector>

#include "Application.h"
#include "Mesh.h"

namespace Margit {

	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

	bool firstMouse = true;

	float lastX, lastY;

	float textureVisibility = 0.0f;

	bool isCursorEnabled = true;

	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	int Application::run() {
		std::cout << "Welcome to Margit Game Engine..." << std::endl << std::endl << "An open-source game engine written in OpenGL 4.6. Author is Nathan Baylon." << std::endl;

		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

		GLFWmonitor* myMonitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(myMonitor);
		int screen_width = mode->width;
		int screen_height = mode->height;

		lastX = screen_width / 2.0f, lastY = screen_height / 2.0f;

		GLFWwindow* window = glfwCreateWindow(screen_width, screen_height, "Margit Game Engine", glfwGetPrimaryMonitor(), nullptr);

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

		// Noise generator
		FastNoiseLite noise;
		noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
		noise.SetFrequency(0.04f);

		glViewport(0, 0, screen_width, screen_height);
		glEnable(GL_DEPTH_TEST);

		// Face culling
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		float cube[] = {
			// Back face
			-0.5f, -0.5f, -0.5f,  0.0f, 0.3333f, // Bottom-left
			 0.5f,  0.5f, -0.5f,  1.0f, 0.6667f, // top-right
			 0.5f, -0.5f, -0.5f,  1.0f, 0.3333f, // bottom-right         
			 0.5f,  0.5f, -0.5f,  1.0f, 0.6667f, // top-right
			-0.5f, -0.5f, -0.5f,  0.0f, 0.3333f, // bottom-left
			-0.5f,  0.5f, -0.5f,  0.0f, 0.6667f, // top-left
			// Front face
			-0.5f, -0.5f,  0.5f,  0.0f, 0.3333f, // bottom-left
			 0.5f, -0.5f,  0.5f,  1.0f, 0.3333f, // bottom-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // top-right
			 0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // top-right
			-0.5f,  0.5f,  0.5f,  0.0f, 0.6667f, // top-left
			-0.5f, -0.5f,  0.5f,  0.0f, 0.3333f, // bottom-left
			// Left face
			-0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // top-right
			-0.5f,  0.5f, -0.5f,  0.0f, 0.6667f, // top-left
			-0.5f, -0.5f, -0.5f,  0.0f, 0.3333f, // bottom-left
			-0.5f, -0.5f, -0.5f,  0.0f, 0.3333f, // bottom-left
			-0.5f, -0.5f,  0.5f,  1.0f, 0.3333f, // bottom-right
			-0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // top-right
			// Right face
			 0.5f,  0.5f,  0.5f,  0.0f, 0.6667f, // top-left
			 0.5f, -0.5f, -0.5f,  1.0f, 0.3333f, // bottom-right
			 0.5f,  0.5f, -0.5f,  1.0f, 0.6667f, // top-right         
			 0.5f, -0.5f, -0.5f,  1.0f, 0.3333f, // bottom-right
			 0.5f,  0.5f,  0.5f,  0.0f, 0.6667f, // top-left
			 0.5f, -0.5f,  0.5f,  0.0f, 0.3333f, // bottom-left     
			 // Bottom face
			 -0.5f, -0.5f, -0.5f,  1.0f, 0.3333f, // top-right
			  0.5f, -0.5f, -0.5f,  0.0f, 0.3333f, // top-left
			  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  // bottom-left
			  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	  // bottom-left
			 -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	  // bottom-right
			 -0.5f, -0.5f, -0.5f,  1.0f, 0.3333f, // top-right
			 // Top face
			 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	  // top-left
			  0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // bottom-right
			  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	  // top-right     
			  0.5f,  0.5f,  0.5f,  1.0f, 0.6667f, // bottom-right
			 -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	  // top-left
			 -0.5f,  0.5f,  0.5f,  0.0f, 0.6667f  // bottom-left        
		};

		// Vertex buffer object - generate buffer id, bind it to array buffer and then copy data to the bound buffer
		GLuint VBOs[5];
		glGenBuffers(5, VBOs);

		// Vertex Array Object
		GLuint VAOs[5];
		glGenVertexArrays(5, VAOs);

		// Rectangle (Five)
		glBindVertexArray(VAOs[4]);

		glBindBuffer(GL_ARRAY_BUFFER, VBOs[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		// LOAD TEXTURES
		GLuint grassAtlasTexture;
		glGenTextures(1, &grassAtlasTexture);
		glBindTexture(GL_TEXTURE_2D, grassAtlasTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// load and generate the texture
		stbi_set_flip_vertically_on_load(true);
		int t_width, t_height, nrChannels;
		unsigned char* data = stbi_load("Textures/grass_atlas.png", &t_width, &t_height, &nrChannels, 0);

		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		// texture 2
		GLuint waterTexture;
		glGenTextures(1, &waterTexture);
		glBindTexture(GL_TEXTURE_2D, waterTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		data = stbi_load("Textures/water.bmp", &t_width, &t_height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t_width, t_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		// dirt block atlas
		GLuint dirtAtlasTexture;
		glGenTextures(1, &dirtAtlasTexture);
		glBindTexture(GL_TEXTURE_2D, dirtAtlasTexture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		data = stbi_load("Textures/dirt_atlas.png", &t_width, &t_height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		Margit::Shader grassShader("Shaders/transform.vert", "Shaders/texture.frag");
		Margit::Shader dirtShader("Shaders/transform.vert", "Shaders/texture.frag");

		dirtShader.use();

		// Bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, dirtAtlasTexture);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, waterTexture);

		dirtShader.setInt("texture1", 0);
		dirtShader.setInt("texture2", 1);

		std::vector<float> vs = {
			-0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.5f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f,
			-0.5f, 0.5f, 0.0f,		1.0f, 1.0f, 1.0f,
		};
		std::vector<unsigned int> is = {
			0, 1, 2,
			0, 2, 3
		};

		std::vector<VertexAttribute> layout = {
			{ 0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0 },
			{ 1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 3 * sizeof(float) }
		};

		Margit::Shader myShader("Shaders/Mesh.vert", "Shaders/Mesh.frag");
		Margit::Mesh myMesh(vs, layout, is);

		while (!glfwWindowShouldClose(window)) {
			// delta time calculation
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			processInput(window);

			myMesh.render(myShader);

			dirtShader.use();
			glBindVertexArray(VAOs[4]);

			dirtShader.setFloat("textureVisibility", textureVisibility);

			// camera logic
			glm::mat4 projection;
			projection = camera.GetPerspectiveMatrix(static_cast<float>(screen_width), static_cast<float>(screen_height));
			dirtShader.setMat4("projection", projection);

			glm::mat4 view = camera.GetViewMatrix();
			dirtShader.setMat4("view", view);

			for (int chunkWidth = 0; chunkWidth < 64; chunkWidth++) {
				for (int chunkDepth = 0; chunkDepth < 64; chunkDepth++) {

					float noiseValue = noise.GetNoise((float)chunkWidth, (float)chunkDepth);
					float chunkPillarHeight = (noiseValue - -1.0f) / (1.0f - -1.0f) * (15 - 0) + 0; // TODO: Turn this into a remap function

					for (int chunkHeight = 0; chunkHeight < chunkPillarHeight; chunkHeight++) {
						glm::mat4 model = glm::mat4(1.0f);

						model = glm::translate(model, glm::vec3((float)chunkWidth, (float)chunkHeight - 16, (float)-chunkDepth));

						if (chunkHeight < chunkPillarHeight - 1) {
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, dirtAtlasTexture);
						}
						else {
							glActiveTexture(GL_TEXTURE0);
							glBindTexture(GL_TEXTURE_2D, grassAtlasTexture);
						}

						dirtShader.setMat4("model", model);

						glDrawArrays(GL_TRIANGLES, 0, 36);
					}
				}
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		return 0;
	}

	Application::~Application() {
		glfwTerminate();
	}

	void Application::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	}

	void Application::processInput(GLFWwindow* window) {
		float cameraSpeed = 10.0f * deltaTime;

		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
			textureVisibility += 1.0f * deltaTime;

			if (textureVisibility >= 1.0f)
				textureVisibility = 1.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			textureVisibility -= 1.0f * deltaTime;

			if (textureVisibility <= 0.0f)
				textureVisibility = 0.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
			isCursorEnabled = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
			isCursorEnabled = true;
			firstMouse = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::TILT_LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::TILT_RIGHT, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::UP, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::DOWN, deltaTime);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(Margit::RIGHT, deltaTime);
	}

	void Application::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (!isCursorEnabled)
			return;

		if (firstMouse) {
			lastX = static_cast<float>(xpos);
			lastY = static_cast<float>(ypos);

			firstMouse = false;
		}

		float xoffset = static_cast<float>(xpos) - lastX;
		float yoffset = lastY - static_cast<float>(ypos); // reversed as y-coordinates range from bottom to top

		lastX = static_cast<float>(xpos);
		lastY = static_cast<float>(ypos);

		camera.ProcessMouseMovement(xoffset, yoffset);
	}

	void Application::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
		camera.ProcessMouseScroll(static_cast<float>(yoffset));
	}
}

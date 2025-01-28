#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Margit {

	class Shader
	{
	public:
		Shader(const char* vertexPath, const char* fragmentPath);

		GLuint getID() const;

		void use();
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, glm::mat4 value) const;

	private:
		GLuint ID;
	};
}

 
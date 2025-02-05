#pragma once
#include <glad/glad.h>
#include <vector>

namespace Margit {
	class Mesh
	{
	public:
		Mesh(std::vector<float> vertices) : Vertices(vertices) {
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);
			glGenBuffers(1, &EBO);
		};

		virtual ~Mesh() {
			glDeleteVertexArrays(1, &VAO);
		}

		void setupMesh() {
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size(), &Vertices[0], GL_STATIC_DRAW);

			//glBindVertexArray(VAO);
		}

		void render() {
			glDrawArrays(GL_TRIANGLES, 0, Vertices.size());
		}

	protected:
		std::vector<float> Vertices;
		GLuint VBO, VAO, EBO;
	};
}


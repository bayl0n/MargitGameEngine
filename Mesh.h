#pragma once
#include <glad/glad.h>
#include <vector>

namespace Margit {
	class Mesh
	{
	public:
		Mesh(std::vector<float> vertices, std::vector<unsigned int> indices = {}) : Vertices(vertices), Indices(indices), EBO(0) {
			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

			// Set up EBO depending if indices are provided
			if (!Indices.empty()) {
				useEBO = true;

				glGenBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int),
					Indices.data(), GL_STATIC_DRAW);
			}
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			glBindVertexArray(0);
		}

		virtual ~Mesh() {
			glDeleteVertexArrays(1, &VAO);
			glDeleteBuffers(1, &VBO);
			if (useEBO && EBO != 0) {
				glDeleteBuffers(1, &EBO);
			}
		}

		void render() {
			glBindVertexArray(VAO);

			if (useEBO)
				glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0);
			else
				glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(Vertices.size()) / 3);

			glBindVertexArray(0);
		}

	protected:
		std::vector<float> Vertices;
		std::vector<unsigned int> Indices;

		bool useEBO = false;
		
		GLuint VBO, VAO, EBO;
	};
}


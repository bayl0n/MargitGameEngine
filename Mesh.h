#pragma once
#include <glad/glad.h>
#include <vector>


namespace Margit {
	struct VertexAttribute {
		GLuint index;
		GLint size;
		GLenum type;
		GLboolean normalized;
		GLsizei stride;
		size_t offset;
	};
	
	class Mesh
	{
	public:
		Mesh(
			const std::vector<float> vertices,
			const std::vector<unsigned int> indices = {}
		)
			:
			Vertices(vertices),
			Indices(indices),
			EBO(0) {

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

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

		Mesh(
			const std::vector<float> vertices,
			const std::vector<VertexAttribute>& layout,
			const std::vector<unsigned int> indices = {}
		) : Vertices(vertices),
			Indices(indices), EBO(0) {

			glGenVertexArrays(1, &VAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(VAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(float), &Vertices[0], GL_STATIC_DRAW);

			if (!Indices.empty()) {
				useEBO = true;

				glGenBuffers(1, &EBO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int),
					Indices.data(), GL_STATIC_DRAW);
			}

			for (const auto& attribute : layout) {
				glVertexAttribPointer(
					attribute.index,
					attribute.size,
					attribute.type,
					attribute.normalized,
					attribute.stride,
					reinterpret_cast<void*>(attribute.offset)
				);

				glEnableVertexAttribArray(attribute.index);
			}

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
			else {
				GLsizei vertexCount = static_cast<GLsizei>(Vertices.size() / (layoutStride / sizeof(float)));
				
				glDrawArrays(GL_TRIANGLES, 0, vertexCount);
			}

			glBindVertexArray(0);
		}

		void setLayoutStride(GLuint stride) {
			layoutStride = stride;
		}

		void render(Shader& shader) {
			shader.use();

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
		GLuint layoutStride = 0;
		GLuint VBO, VAO, EBO;
	};
}


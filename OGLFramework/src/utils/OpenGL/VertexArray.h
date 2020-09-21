#pragma once

#include <cstdint>
#include <vector>

#include <glm/glm.hpp>

#include <GLAD/glad.h>

namespace OGLFramework
{
	class VertexArray
	{
	public:
		VertexArray(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices);
		VertexArray(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices);

		VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size, uint32_t* indices, size_t indicies_size);
		VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size, uint32_t vertices_count);
		VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size);

		GLuint GetVAO() { return m_VAO; }
		GLuint GetVBO() { return m_VBO; }
		GLuint GetEBO() { return m_EBO; }

		uint32_t GetIndicesCount() { return m_IndexCount; }

		explicit operator GLuint() { return m_VAO; }

	private:
		void LoadData(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices);

		GLuint m_VAO, m_VBO, m_EBO;
		uint32_t m_IndexCount;
	};
}
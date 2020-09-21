#include "VertexArray.h"

#include <spdlog/spdlog.h>

namespace OGLFramework
{
	VertexArray::VertexArray(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices)
	{
		LoadData(attribs, vertices, indices);
	}

	VertexArray::VertexArray(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices)
	{
		std::vector<uint32_t> emptyVector;
		LoadData(attribs, vertices, emptyVector);
	}

	VertexArray::VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size, uint32_t* indices, size_t indicies_size)
	{
		std::vector<glm::vec3> verticesVector;
		std::vector<glm::uint32_t> indicesVector;

		uint32_t lpv = 0; // lenght per vertex
		for (auto it : attribs)
			lpv += it;

		assert(vertices_size % 3 == 0); // vertices size is not a multiple of lpv

		for (size_t i = 0; i < vertices_size; i += 3)
		{
			glm::vec3 vertex;

			vertex.x = vertices[i];
			vertex.y = vertices[i + 1];
			vertex.z = vertices[i + 2];

			verticesVector.push_back(vertex);
		}

		for (size_t i = 0; i < indicies_size; i++)
			indicesVector.push_back(indices[i]);

		LoadData(attribs, verticesVector, indicesVector);
	}

	VertexArray::VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size, uint32_t vertices_count)
	{
		std::vector<glm::vec3> verticesVector;
		std::vector<uint32_t> emptyVector;

		uint32_t lpv = 0; // lenght per vertex
		for (auto it : attribs)
			lpv += it;

		assert(vertices_size % lpv == 0); // vertices size is not a multiple of lpv

		for (size_t i = 0; i < vertices_size; i += 3)
		{
			glm::vec3 vertex;

			vertex.x = vertices[i];
			vertex.y = vertices[i + 1];
			vertex.z = vertices[i + 2];

			verticesVector.push_back(vertex);
		}

		m_IndexCount = vertices_count; // Only for common draw
		LoadData(attribs, verticesVector, emptyVector);
	}

	VertexArray::VertexArray(const std::vector<uint32_t>& attribs, float* vertices, size_t vertices_size)
	{
		std::vector<glm::vec3> verticesVector;
		std::vector<uint32_t> emptyVector;

		uint32_t lpv = 0; // lenght per vertex
		for (auto it : attribs)
			lpv += it;

		assert(vertices_size % lpv == 0); // vertices size is not a multiple of lpv

		for (size_t i = 0; i < vertices_size; i += 3)
		{
			glm::vec3 vertex;

			vertex.x = vertices[i];
			vertex.y = vertices[i + 1];
			vertex.z = vertices[i + 2];

			verticesVector.push_back(vertex);
		}

		LoadData(attribs, verticesVector, emptyVector);
	}

	void VertexArray::LoadData(const std::vector<uint32_t>& attribs, const std::vector<glm::vec3>& vertices, const std::vector<uint32_t>& indices)
	{
		glCreateVertexArrays(1, &m_VAO);
		glBindVertexArray(m_VAO);

		glCreateBuffers(1, &m_VBO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_STATIC_DRAW);

		if (indices.size())
		{
			glCreateBuffers(1, &m_EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), &indices[0], GL_STATIC_DRAW);

			m_IndexCount = indices.size();
		}

		size_t offset = 0;
		size_t stride = 0;

		for (size_t i = 0; i < attribs.size(); i++)
			stride += attribs[i] * sizeof(float);

		for (size_t i = 0; i < attribs.size(); i++)
		{
			glEnableVertexAttribArray(i);
			uint32_t attrib = attribs[i];
			glVertexAttribPointer(i, attrib, GL_FLOAT, GL_FALSE, stride, (void*)offset);

			offset += (attribs[i] * sizeof(float));
		}

		glBindVertexArray(0);
	}
}
#pragma once

#pragma once
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>

namespace OGLFramework
{
	class Shader {
	public:
		Shader() = default;
		Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

		void Compile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);

		void Use();
		
		void SetFloat(const std::string& name, float value);
		void SetBool(const std::string& name, bool value);
		void SetInt(const std::string& name, int value);

		void SetVec2(const std::string& name, const glm::vec2& value);
		void SetVec3(const std::string& name, const glm::vec3& value);
		void SetVec4(const std::string& name, const glm::vec4& value);

		void SetMat2(const std::string& name, const glm::mat3& mat);
		void SetMat3(const std::string& name, const glm::mat3& mat);
		void SetMat4(const std::string& name, const glm::mat4& mat);
	private:
		void CheckCompileErrors(GLuint shader, const std::string& type);

		GLuint m_RendererID;
	};
}
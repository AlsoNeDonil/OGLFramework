#pragma once

#include <spdlog/spdlog.h>
#include <stb_image.h>
#include <GLAD/glad.h>
#include <string>

namespace OGLFramework
{
	class Texture
	{
	public:
		Texture(const std::string& path);

		void Set(uint32_t number);

	private:
		void Load(const std::string& path);

		uint32_t m_Width, m_Height;
		GLuint m_RendererID;
		std::string m_Path;
	};
}

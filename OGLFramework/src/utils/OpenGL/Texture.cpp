#include "Texture.h"

OGLFramework::Texture::Texture(const std::string& path)
	: m_Path(path)
{
	Load(path);
}

void OGLFramework::Texture::Set(uint32_t number)
{
	glActiveTexture(GL_TEXTURE0 + number);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void OGLFramework::Texture::Load(const std::string& path)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);

	int width, height, nrComponents;

	stbi_uc* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	else
		spdlog::error("Failed to load texture: {0}", path.c_str());

	stbi_image_free(data);
}

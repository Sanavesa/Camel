#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

namespace Camel
{
	Texture::Texture(const std::string& filePath, const FilterMode filterMode)
		: m_TextureID(0), m_Width(0), m_Height(0), m_NumChannels(0)
	{
		stbi_set_flip_vertically_on_load(1);
		auto imageBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_NumChannels, 4);
		if (!imageBuffer)
		{
			CAMEL_LOG_ERROR("Failed to load texture from path: {}", filePath);
			throw std::runtime_error("Failed to load texture from path: " + filePath);
		}

		glGenTextures(1, &m_TextureID);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		// Minification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLenum)filterMode);

		// Magnification filter
		if (filterMode == FilterMode::LINEAR || filterMode == FilterMode::LINEAR_MIPMAP_NEAREST || filterMode == FilterMode::LINEAR_MIPMAP_LINEAR)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageBuffer);

		// Generate mipmaps
		if (filterMode == FilterMode::NEAREST_MIPMAP_NEAREST || filterMode == FilterMode::LINEAR_MIPMAP_NEAREST || filterMode == FilterMode::NEAREST_MIPMAP_LINEAR || filterMode == FilterMode::LINEAR_MIPMAP_LINEAR)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		// Unbind
		glBindTexture(GL_TEXTURE_2D, 0);

		if (imageBuffer)
			stbi_image_free(imageBuffer);
	}

	Texture::~Texture() noexcept
	{
		glDeleteTextures(1, &m_TextureID);
	}
}

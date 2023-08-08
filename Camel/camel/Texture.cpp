#include "Texture.h"

#include "vendor/stb_image/stb_image.h"

namespace Camel
{
	Texture Texture::Load(const std::string& filePath, const Texture::FilterMode filterMode)
	{
		stbi_set_flip_vertically_on_load(1);
		int width, height, numChannels;
		auto imageBuffer = stbi_load(filePath.c_str(), &width, &height, &numChannels, 4);
		if (!imageBuffer)
		{
			CAMEL_LOG_ERROR("Failed to load texture from path: {}", filePath);
			throw std::runtime_error("Failed to load texture from path: " + filePath);
		}

		Texture texture(width, height, numChannels, filterMode, imageBuffer);
		stbi_image_free(imageBuffer);
		return texture;
	}

	Texture::Texture(const int width, const int height, const int numChannels, const FilterMode filterMode, const unsigned char* imageBuffer)
		: m_Width(width), m_Height(height), m_NumChannels(numChannels)
	{
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

		// If imageBuffer is provided, fill m_PixelData from imageBuffer
		if (imageBuffer)
			m_PixelData = std::vector<unsigned char>(imageBuffer, imageBuffer + m_Width * m_Height * m_NumChannels);
		// If imageBuffer is not provided, reserve space for m_PixelData with default values
		else
			m_PixelData.resize(m_Width * m_Height * m_NumChannels, 0);  // Initialize to transparent black
	}

	Texture::Texture(Texture&& other) noexcept
		: m_TextureID(other.m_TextureID), m_Width(other.m_Width), m_Height(other.m_Height), m_NumChannels(other.m_NumChannels), m_PixelData(std::move(other.m_PixelData))
	{
		other.m_TextureID = 0;
	}


	Texture& Texture::operator=(Texture&& other) noexcept
	{
		if (this != &other)
		{
			glDeleteTextures(1, &m_TextureID);

			m_TextureID = other.m_TextureID;
			m_Width = other.m_Width;
			m_Height = other.m_Height;
			m_NumChannels = other.m_NumChannels;
			m_PixelData = std::move(other.m_PixelData);

			other.m_TextureID = 0;
		}
		return *this;
	}

	Texture::~Texture() noexcept
	{
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
	{
		int index = (y * m_Width + x) * m_NumChannels;
		m_PixelData[index] = r;
		m_PixelData[index + 1] = g;
		m_PixelData[index + 2] = b;
		m_PixelData[index + 3] = a;
	}

	void Texture::UpdateTexture()
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, GL_RGBA, GL_UNSIGNED_BYTE, m_PixelData.data());
	}
}

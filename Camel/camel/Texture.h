#pragma once

#include "Core.h"

#include <vector>

namespace Camel
{
	class Texture final
	{
	public:
		enum class FilterMode
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
		};

	public:
		static Texture Load(const std::string& filePath, const FilterMode filterMode = FilterMode::LINEAR);

	public:
		Texture(const int width, const int height, const int numChannels, const FilterMode filterMode = FilterMode::LINEAR, const unsigned char* imageBuffer = nullptr);

		Texture(const Texture&) = delete;
		Texture& operator=(const Texture&) = delete;

		Texture(Texture&& other) noexcept;
		Texture& operator=(Texture&& other) noexcept;

		~Texture() noexcept;

		inline void Bind(unsigned int slot = 0) const noexcept
		{
			CAMEL_ASSERT(slot >= 0 && slot <= 31, "Cannot bind texture to slot {}. Acceptable values are 0 to 31.", slot);
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, m_TextureID);
		}

		inline void Unbind() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		inline int GetWidth() const noexcept { return m_Width; }
		inline int GetHeight() const noexcept { return m_Height; }
		inline int GetNumChannels() const noexcept { return m_NumChannels; }

		void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		void UpdateTexture();

	private:
		unsigned int m_TextureID;
		int m_Width, m_Height, m_NumChannels;
		std::vector<unsigned char> m_PixelData; // Store pixel data in system memory
	};
}

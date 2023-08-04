#pragma once

#include "Core.h"

namespace Camel
{
	class Texture
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

		Texture(const std::string& filePath, const FilterMode filterMode = FilterMode::LINEAR);
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

	private:
		unsigned int m_TextureID;
		int m_Width, m_Height, m_NumChannels;
	};
}

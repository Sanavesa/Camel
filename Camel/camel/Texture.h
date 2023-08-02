#pragma once

#include <string>
#include <GL/glew.h>

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

	Texture(const std::string& path, const FilterMode filterMode = FilterMode::LINEAR);
	~Texture() noexcept;

	void Bind(unsigned int slot = 0) const noexcept
	{
		if (slot < 0 || slot > 31)
			slot = 0;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, m_TextureID);
	}

	void Unbind() const noexcept
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}


	inline int GetWidth() const noexcept { return m_Width; }
	inline int GetHeight() const noexcept { return m_Height; }

private:
	unsigned int m_TextureID;
	std::string m_FilePath;
	int m_Width, m_Height, m_BPP;
};

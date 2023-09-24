#include "Texture2D.h"
#include <stb_image/stb_image.h>

#include <iostream>
#include <glad/glad.h>
#include <filesystem>

Texture2D::Texture2D()
	: m_RendererID(0), m_Filepath(""), m_Channels(4)
{
	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(void* data, uint32_t width, uint32_t height, uint32_t channels)
	: m_RendererID(0), m_Filepath(""), m_Width(width), m_Height(height), m_Channels(channels)
{
	switch (channels)
	{
		case 1:
			m_InternalFormat = GL_R8;
			m_DataFormat = GL_RED;
			break;
		case 3:
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
			break;
		case 4:
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
			break;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

Texture2D::Texture2D(const std::string& filepath)
	: m_RendererID(0), m_Filepath(filepath), m_Channels(0)
{
	
	if (!std::filesystem::is_regular_file(filepath))
	{
		if (std::filesystem::absolute(filepath).make_preferred() == std::filesystem::path(filepath).make_preferred())
			std::cout << "(ERROR): Couldn't load font file located at " << filepath << std::endl;
		else
			std::cout << "(ERROR): Couldn't load font file located at " << std::filesystem::current_path().string() << '\\' << std::filesystem::path(filepath).make_preferred().string() << std::endl;
		return;
	}

	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 4);

	if (m_Channels == 4)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;
	}
	else if (m_Channels == 3)
	{
		m_InternalFormat = GL_RGB8;
		m_DataFormat = GL_RGB;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);

	if (data)
		stbi_image_free(data);
	else
		std::cout << stbi_failure_reason() << std::endl;
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::SetData(void* data, uint32_t size)
{
	uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
	if (size == m_Width * m_Height * bpp)
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	else
		std::cout << "Incorrect size for data: size = " << size << " , width * height * bpp = " << m_Width * m_Height * bpp;
}

void Texture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}

void Texture2D::Unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

#pragma once
#include <cstdint>
#include <string>

#include "Core.h"

class Texture
{
public:
	Texture();
	Texture(const std::string& filepath);
	~Texture();
	const uint32_t Width() const { return m_Width; }
	const uint32_t Height() const { return m_Height; }
	void SetData(void* data, uint32_t size);
	void Bind(uint32_t slot = 0) const;
	void Unbind() const;
	const uint32_t RendererID() const { return m_RendererID; }
	bool operator==(const Texture& other) const { return m_RendererID == other.m_RendererID; }
private:
	uint32_t m_RendererID;
	std::string m_Filepath;
	int m_Width = 1, m_Height = 1, m_Channels;
	uint32_t m_InternalFormat, m_DataFormat;
};

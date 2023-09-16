#pragma once
#include <cstdint>
#include <string>

#include "Core.h"

class Texture2D
{
public:
	Texture2D();
	Texture2D(void* data, uint32_t width, uint32_t height, uint32_t channels);
	Texture2D(const std::string& filepath);
	~Texture2D();
	const uint32_t Width() const { return m_Width; }
	const uint32_t Height() const { return m_Height; }
	void SetData(void* data, uint32_t size);
	void Bind(uint32_t slot = 0) const;
	void Unbind() const;
	const uint32_t RendererID() const { return m_RendererID; }
	bool operator==(const Texture2D& other) const { return m_RendererID == other.m_RendererID; }
private:
	uint32_t m_RendererID;
	std::string m_Filepath;
	int m_Width = 1, m_Height = 1, m_Channels;
	uint32_t m_InternalFormat, m_DataFormat;
};

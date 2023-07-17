#pragma once
#include <stdint.h>
#include <string>

class Texture
{
public:
	Texture(const std::string& filepath);
	~Texture();
	void Bind(uint32_t slot = 0) const;
	void Unbind() const;
	const uint32_t Width() const { return m_Width; }
	const uint32_t Height() const { return m_Height; }
private:
	uint32_t m_RendererID;
	std::string m_Filepath;
	uint8_t* m_Localbuffer;
	int m_Width, m_Height, m_BPP;
};

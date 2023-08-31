#include "CanvasTexture.h"
#include "Texture.h"

CanvasTexture::CanvasTexture()
	: m_InternalTexture(nullptr)
{
}

CanvasTexture::CanvasTexture(const std::string& filepath)
{
	m_InternalTexture = new Texture(filepath);
}

CanvasTexture::~CanvasTexture()
{
	delete m_InternalTexture;
}

const uint32_t CanvasTexture::Width() const
{
	return m_InternalTexture->Width();
}

const uint32_t CanvasTexture::Height() const
{
	return m_InternalTexture->Height();
}

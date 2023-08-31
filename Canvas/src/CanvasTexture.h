#pragma once

#include <string>
#include "Core.h"

class Texture;

class CANVAS_API CanvasTexture
{
public:
	CanvasTexture();
	CanvasTexture(const std::string& filepath);
	~CanvasTexture();
	const uint32_t Width() const;
	const uint32_t Height() const;
	const Texture* InternalTexture() const { return m_InternalTexture; }
private:
	Texture* m_InternalTexture;
};


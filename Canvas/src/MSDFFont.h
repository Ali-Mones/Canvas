#pragma once

#include <msdf-atlas-gen.h>
#include <string>

#include "Texture2D.h"

struct MSDFFont
{
	MSDFFont(const char* filepath);
	~MSDFFont();
	msdf_atlas::FontGeometry FontGeometry;
	std::vector<msdf_atlas::GlyphGeometry> Glyphs;
	Texture2D* FontAtlas;
};

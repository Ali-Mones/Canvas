#include "MSDFFont.h"

#include <msdfgen.h>
#include <msdfgen-ext.h>
#include <msdf-atlas-gen.h>
#include <cassert>
#include <iostream>
#include <filesystem>

MSDFFont::MSDFFont(const char* filepath)
{
	if (!std::filesystem::is_regular_file(filepath))
	{
		if (std::filesystem::absolute(filepath).make_preferred() == std::filesystem::path(filepath).make_preferred())
			std::cout << "(ERROR): Couldn't load font file located at " << filepath << std::endl;
		else
			std::cout << "(ERROR): Couldn't load font file located at " << std::filesystem::current_path().string() << '\\' << std::filesystem::path(filepath).make_preferred().string() << std::endl;
		return;
	}

	if (msdfgen::FreetypeHandle* ft = msdfgen::initializeFreetype())
	{
		// Load font file
		if (msdfgen::FontHandle* font = msdfgen::loadFont(ft, filepath))
		{
			// Storage for glyph geometry and their coordinates in the atlas
			FontGeometry = msdf_atlas::FontGeometry(&Glyphs);
			int loaded = FontGeometry.loadCharset(font, 1.0, msdf_atlas::Charset::ASCII);
			assert(loaded == 95);

			const double maxCornerAngle = 3.0;
			for (msdf_atlas::GlyphGeometry& glyph : Glyphs)
				glyph.edgeColoring(&msdfgen::edgeColoringInkTrap, maxCornerAngle, 0);

			// TightAtlasPacker class computes the layout of the atlas.
			msdf_atlas::TightAtlasPacker packer;
			packer.setDimensionsConstraint(msdf_atlas::TightAtlasPacker::DimensionsConstraint::MULTIPLE_OF_FOUR_SQUARE);
			packer.setScale(40.0);
			packer.setPixelRange(2.0);
			packer.setMiterLimit(1.0);
			packer.setPadding(0.0);

			// Compute atlas layout - pack glyphs
			int remaining = packer.pack(Glyphs.data(), Glyphs.size());
			assert(remaining == 0);

			// Get final atlas dimensions
			int width, height;
			packer.getDimensions(width, height);

			msdf_atlas::ImmediateAtlasGenerator<float, 3, msdf_atlas::msdfGenerator, msdf_atlas::BitmapAtlasStorage<msdfgen::byte, 3>> generator(width, height);
			generator.setAttributes(msdf_atlas::GeneratorAttributes());
			generator.setThreadCount(8);
			generator.generate(Glyphs.data(), Glyphs.size());

			msdfgen::BitmapConstRef<msdfgen::byte, 3> bitmap = (msdfgen::BitmapConstRef<msdfgen::byte, 3>) generator.atlasStorage();
			FontAtlas = new Texture2D((void*)bitmap.pixels, width, height, 3);

			msdfgen::destroyFont(font);
		}
		msdfgen::deinitializeFreetype(ft);
	}
}

MSDFFont::~MSDFFont()
{
	delete FontAtlas;
}

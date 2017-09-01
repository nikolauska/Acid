﻿#include "fonttype.hpp"

namespace Flounder
{
	FontType::FontType(const std::string &textureFile, const std::string &fontFile) :
		m_texture(new texture(textureFile)),
		m_metadata(new Metafile(fontFile))
	{
	}

	FontType::~FontType()
	{
		delete m_texture;
		delete m_metadata;
	}
}

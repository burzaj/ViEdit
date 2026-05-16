#pragma once

#include <vector>
#include <string>
#include <unordered_map>

#include "Frame.h"
#include "Color.h"
#include "Effect.h"

struct CachedGlyph {
	std::vector<unsigned char> bitmap;
	int width, height;
	int x_offset, y_offset;
	int advance;
};

class Text : public Effect
{
	float x = 0.0f, y = 0.0f;
	bool x_scaled = false, y_scaled = false;
	bool init = false;
	std::string text;

	bool v_align_centered = false,
		 h_align_centered = false;

	Color color = {255,255,255};

	std::unordered_map<unsigned char, CachedGlyph> glyph_cache;
	int font_ascent;
	float font_scale;
	float half_text_width = 0.0f;

public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Text(std::string arg);
};
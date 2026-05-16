#pragma once

#include <string>
#include <vector>

#include "Effect.h"
#include "Color.h"
#include "Process.h"

#define LOOKUP_PALLETE_RESOLUTION 16

void calculate_palette_lookup(std::vector<Color>& palette,
	size_t palette_lookup[LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION]);

class Palette : public Effect
{
	std::vector<Color> palette;

	size_t palette_lookup[LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION];

	unsigned char divisor;

public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Palette(std::string args);
};


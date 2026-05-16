#pragma once

#include <vector>
#include <string>

#include "Effect.h"
#include "Frame.h"

class MaxBrightness : public Effect
{
	unsigned char brightness;
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	MaxBrightness(std::string arg);
};


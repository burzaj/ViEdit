#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Effect.h"

void distribute_error(Frame& frame, const size_t& x, const size_t& y, float error);

class AtkinsonDithering : public Effect
{
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	AtkinsonDithering(std::string arg);
};
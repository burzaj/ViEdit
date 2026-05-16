#pragma once

#include <vector>
#include <string>

#include "Effect.h"
#include "Frame.h"
#include "Process.h"

class AtkinsonColorDithering : public Effect
{
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	AtkinsonColorDithering(std::string arg);
};


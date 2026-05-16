#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Effect.h"

class Skip : public Effect
{
	float stagger = 0;
	int curr_count = 0;
	bool init = true;
	int numb_of_effects_to_skip = 2147483647;
	bool reversed = false;
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Skip(std::string arg);
};


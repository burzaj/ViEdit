#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Effect.h"

class Save : public Effect
{
	float stagger = 0;
	int curr_count = 0;
	bool init = true;
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Save(std::string arg);
};


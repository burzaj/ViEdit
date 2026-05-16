#pragma once

#include <vector>
#include <string>

#include "Effect.h"
#include "Frame.h"

class Reset : public Effect
{
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Reset(std::string arg);
};


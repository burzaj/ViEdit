#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Color.h"
#include "Effect.h"

class Rectangle : public Effect
{
	float x1 = 0.0f, x2 = 0.0f, y1 = 0.0f, y2 = 0.0f;
	bool x1_scaled = false, x2_scaled = false, y1_scaled = false, y2_scaled = false;

	bool filled = true;

	Color color = {0,0,0};

	bool init = false;

public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Rectangle(std::string arg);
};
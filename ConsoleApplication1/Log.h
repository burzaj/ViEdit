#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Effect.h"

class Log : public Effect
{
	double real = 1.0;
	double imag = 1.0;
	double h_offset = 0.5;
	double w_offset = 0.5;
	double scale = 1;
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Log(std::string arg);
};


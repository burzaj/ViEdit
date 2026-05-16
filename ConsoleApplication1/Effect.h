#pragma once

#include <vector>
#include <string>

#include "Frame.h"
#include "Process.h"
#include "memory"

class Effect
{
public:
	virtual void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) = 0;

	~Effect() {};
};
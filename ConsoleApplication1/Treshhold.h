#pragma once

#include "Effect.h"

class Treshhold : public Effect
{
	short treshhold;
public:
	void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

	Treshhold(std::string arg);
};


#pragma once

#include "GlobalEffect.h"
#include <string>

class Scale : public GlobalEffect
{
	size_t width;
	size_t height;
	float scale;
public:
	void apply(Process& process) override;

	Scale(std::string arg);
};


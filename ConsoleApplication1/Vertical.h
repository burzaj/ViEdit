#pragma once
#include "GlobalEffect.h"
#include <string>

class Vertical : public GlobalEffect
{
public:
	void apply(Process& process) override;

	Vertical(std::string arg);
};

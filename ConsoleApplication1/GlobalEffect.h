#pragma once

class Process;

class GlobalEffect
{
public:
	virtual void apply(Process& process) = 0;

	virtual ~GlobalEffect();
};
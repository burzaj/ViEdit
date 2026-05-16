#include "Vertical.h"

#include "Process.h"

void Vertical::apply(Process& process)
{
	process.vertical = true;
}

Vertical::Vertical(std::string arg)
{
}

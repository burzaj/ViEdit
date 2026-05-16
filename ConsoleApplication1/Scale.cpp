#include "Scale.h"
#include "initialization.h"

#include "Macros.h"
#include "Process.h"

void Scale::apply(Process& process)
{
	if (scale == 0.0f)
	{
		process.height = height;
		process.width = width;
	}
	else
	{
		process.height = static_cast<size_t>(std::roundf(process.height * scale));
		process.width = static_cast<size_t>(std::roundf(process.width * scale));

		process.width -= process.width & 1 ? 1 : 0;
		process.height -= process.height & 1 ? 1 : 0;
	}
}

Scale::Scale(std::string arg)
{
	width = 0;
	height = 0;

	scale = 0.0f;

	auto args = split_string(arg, ARG_SEPARATOR);
	
	if (args.size() > 2)
	{
		width = std::stoi(args[0]);
		height = std::stoi(args[1]);

		width -= width & 1 ? 1 : 0;
		height -= height & 1 ? 1 : 0;
	}
	else if (args.size() == 1)
	{
		scale = std::stof(args[0]);
	}
}
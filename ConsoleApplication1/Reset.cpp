#include "Reset.h"

void Reset::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
	frame = original_frame;
}

Reset::Reset(std::string arg)
{
}

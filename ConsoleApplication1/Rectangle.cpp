#include "Rectangle.h"

#include "initialization.h"
#include "Macros.h"
#include "Math.h"

void Rectangle::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
	if (!init)
	{
		if (x1_scaled)
			x1 = clamp(width * x1, 0, width-1);
		if (x2_scaled)
			x2 = clamp(width * x2, 0, width-1);
		if (y1_scaled)
			y1 = clamp(height * y1, 0, height-1);
		if (y2_scaled)
			y2 = clamp(height * y2, 0, height-1);

		init = true;
	}

	int x1_int = x1, 
		x2_int = x2, 
		y1_int = y1, 
		y2_int = y2;

	if (filled)
	{
#pragma omp parallel for
		for (int y = y1_int; y <= y2_int; ++y)
			for (int x = x1_int; x <= x2_int; ++x)
			{
				frame(x, y) = color;
			}
	}
	else
	{
#pragma omp parallel for
		for (int y = y1_int; y <= y2_int; ++y)
		{
			frame(x1_int, y) = color;
			frame(x2_int, y) = color;
		}

#pragma omp parallel for
		for (int x = x1_int +1; x <= x2_int -1; ++x)
		{
			frame(x, y1_int) = color;
			frame(x, y2_int) = color;
		}
	}
}

Rectangle::Rectangle(std::string arg)
{
	auto args = strip_string_vector(split_string(arg, ARG_SEPARATOR));

	int amout_of_args = args.size();

	if (amout_of_args >= 4)
	{
		if (is_str_num(args[0]) and is_str_num(args[1]) and is_str_num(args[2]) and is_str_num(args[3]))
		{
			x1 = std::stof(args[0]);
			x2 = std::stof(args[1]);
			y1 = std::stof(args[2]);
			y2 = std::stof(args[3]);
		}

		if (is_str_float(args[0]))
			x1_scaled = true;
		if (is_str_float(args[1]))
			x2_scaled = true;
		if (is_str_float(args[2]))
			y1_scaled = true;
		if (is_str_float(args[3]))
			y2_scaled = true;

		int col_channel_id = 0;

		for (int i = 4; i < amout_of_args; i++)
		{
			if (args[i] == "line")
				filled = false;
			else if (is_str_int(args[i]))
			{
				switch (col_channel_id)
				{
				case 0:
					color.r = std::stoi(args[i]);
				break;
				case 1:
					color.g = std::stoi(args[i]);
				break;
				case 2:
					color.b = std::stoi(args[i]);
				break;
				}
				col_channel_id++;
			}
		}
	}
}

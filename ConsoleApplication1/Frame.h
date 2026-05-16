#pragma once

#include <vector>
#include "Color.h"

class Frame
{
	size_t f_width;
	size_t f_heigth;
	std::vector<Color> frame;
public:
	size_t get_idx(size_t x, size_t y);

	Color& operator()(const size_t& x, const size_t& y);
	Color& operator()(const size_t& idx);
	unsigned char operator[](const size_t& idx);

	size_t& width();
	size_t& height();
	size_t size();

	unsigned char* data();

	Frame& operator=(const std::vector<unsigned char>& other);

	void construct(const size_t& width, const size_t& height);

	Frame();
};

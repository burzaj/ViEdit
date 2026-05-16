#pragma once

#include <vector>
#include <string>

#include "Macros.h"

void distribute_error(int x, int y, float error);

void distribute_color_error(int x, int y, float errR, float errG, float errB, std::vector<float>& atkinson_flloat);

std::vector<float> create_gaussian_kernel(int radius, float sigma);

float get_dist(int i, int j);

int get_frame_mean(std::vector<unsigned char>& frame, unsigned char& prev_mean);

unsigned char get_density(int block_y, int block_x);

void calculate_pallete_lookup();

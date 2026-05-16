#pragma once

#include <filesystem>
#include <iostream>
#include <cstdio>
#include <memory>

namespace fs = std::filesystem;

void clear_screen();

void get_dimensions(fs::path path, size_t& w, size_t& h, long int& count, const bool& vertical);


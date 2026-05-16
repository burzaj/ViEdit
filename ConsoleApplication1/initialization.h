#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

#include "Effect.h"
#include "GlobalEffect.h"

namespace fs = std::filesystem;

std::string strip_string(const std::string& string, char character = ' ');

std::vector<std::string> strip_string_vector(const std::vector<std::string>& string, char character = ' ');

std::vector<unsigned char> load_file_binary(const fs::path& path);

bool is_str_num(const std::string& string);

bool is_str_int(const std::string& string);

bool is_str_float(const std::string& string);

std::string get_arg(const std::string& a, const std::string& b);

std::vector<std::string> split_string(const std::string& to_split, const char& sep);

bool is_path(std::string path);

std::string clean_path(std::string path);

void try_process_file(std::string raw_path, std::vector<std::string> args);

void parse_cmd_args(std::vector<fs::path>& filepaths,
	std::vector<std::vector<Effect*>>& effects,
	std::vector<std::vector<GlobalEffect*>>& global_effects,
	std::vector<std::string> args);
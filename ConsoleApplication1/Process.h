#pragma once

#include <string>
#include <vector>
#include <filesystem>

#include "Frame.h"
#include "GlobalEffect.h"
#include "process.hpp"

namespace fs = std::filesystem;

class Effect;
class Scale;
class Vertical;

class Process
{
	std::vector<char> stream_buffer;

	std::unique_ptr<TinyProcessLib::Process> ffmpeg_in;
	std::unique_ptr<TinyProcessLib::Process> ffmpeg_out;

	Frame original_frame;
	Frame target_frame;

	size_t width;
	size_t height;

	bool vertical;

	std::wstring input_cmd;
	std::wstring output_cmd;

	fs::path filepath;

	std::wstring output_name;

	std::vector<Effect*> effects;

public:
	std::string start(const bool& terminal = false, const int& curr_proc = -1, const int& max_proc = -1, const int& curr_file = -1, const int& max_file = -1);
	fs::path peek_output();

	long int frames;
	fs::path extension;
	bool stop = false;
	bool skip_frame = false;
	int frame_count = 0;
	int skip_effects = 0;

	Process(fs::path& filepath, std::vector<Effect*>& _effects, std::vector<GlobalEffect*>& _global_effects, const size_t& _num);

	friend Scale;
	friend Vertical;
};


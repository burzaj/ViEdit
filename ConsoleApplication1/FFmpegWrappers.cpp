#define _CRT_SECURE_NO_WARNINGS

#include "FFmpegWrappers.h"

#include <filesystem>
#include <iostream>
#include <cstdio>
#include <memory>

#include "process.hpp"

#include "Macros.h"
#include <cstdlib> // For system()

namespace fs = std::filesystem;

void clear_screen() {
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void get_dimensions(fs::path path, size_t& w, size_t& h, long int& count, const bool& vertical) {
    std::wstring cmd = L"ffprobe -v error -select_streams v:0 -show_entries stream=width,height,nb_frames -of csv=p=0 \"" + path.wstring() + L"\"";

    std::string output;
    std::string error_log;

    TinyProcessLib::Process process(cmd, L"",
        [&output](const char* bytes, size_t n) {
            output += std::string(bytes, n);
        },
        [&error_log](const char* bytes, size_t n) {
            error_log += std::string(bytes, n);
        },
        false
    );

    int exit_status = process.get_exit_status();

    if (exit_status != 0 || output.empty()) {
        std::cerr << "CRITICAL: ffprobe failed or returned no data for " << path << std::endl;
        std::cerr << "Error Log: " << error_log << std::endl;
        exit(1);
    }

    unsigned int width = 0, height = 0;
    long long frames = 0;

    if (sscanf(output.c_str(), "%u,%u,%lld", &width, &height, &frames) < 2) {
        w = 0; h = 0; count = 0;
    }
    else {
        count = (frames);
        if (vertical) {
            h = static_cast<size_t>(width);
            w = static_cast<size_t>(height);
        }
        else {
            w = static_cast<size_t>(width);
            h = static_cast<size_t>(height);
        }
    }

    if (w == 0 || h == 0) {
        std::cerr << "CRITICAL: Could not parse dimensions for " << path << std::endl;
        exit(1);
    }
}

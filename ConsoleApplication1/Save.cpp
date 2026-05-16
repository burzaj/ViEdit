#include "Save.h"

#include "Process.h"
#include "initialization.h"

static void save_frame_as_png(unsigned char* data, int width, int height, std::string filename) {
    // Added -update 1 and -frames:v 1 here as well
    std::string cmd = "ffmpeg -y -f rawvideo -pix_fmt rgb24 -s " +
        std::to_string(width) + "x" + std::to_string(height) +
        " -i - -update 1 -frames:v 1 \"" + filename + ".png\"";

    FILE* pipe = _popen(cmd.c_str(), "wb");
    if (pipe) {
        fwrite(data, 1, width * height * 3, pipe);
        _pclose(pipe);
    }
}

void Save::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
#pragma omp critical(save_snapshot)
    {
        if (!init)
        {
            stagger = process.frames * stagger;
            init = true;
        }

        if (curr_count >= stagger)
        {
            save_frame_as_png(frame.data(), width, height, "Output\\Output_snapshot_" + std::to_string(process.frame_count));
            curr_count = 0;
        }
        else
            curr_count += 1;
    }
}

Save::Save(std::string arg)
{
    curr_count = 0;
    auto striped = strip_string(arg);
    if (is_str_int(striped))
        stagger = std::stoi(striped);
    else if (is_str_float(striped))
    {
        stagger = std::stof(striped);
        init = false;
    }
}

#include "MaxBrightness.h"

#include "initialization.h"
#include "Process.h"

void MaxBrightness::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
#pragma omp parallel for
    for (int i = 0; i < frame.size(); ++i) {

        Color& curr_col = frame(i);
        unsigned char max_ch = std::max({ curr_col.r, curr_col.g, curr_col.b });

        if (max_ch > 0.0f) {
            float scale = brightness / max_ch;
            curr_col *= scale;
        }
    }
}

MaxBrightness::MaxBrightness(std::string arg)
{
    if (is_str_int(strip_string(arg)))
        brightness = std::stoi(arg);
}

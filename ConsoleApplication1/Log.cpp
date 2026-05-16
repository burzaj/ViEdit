#include "Log.h"

#include "Math.h"
#include "initialization.h"
#include "Macros.h"

void Log::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    Frame pre_exp_frame = frame;

    // Pre-calculate constants
    double half_w = width * w_offset;
    double half_h = height * h_offset;

    double inv_w = 1.0 / width;
    double inv_h = 1.0 / height;

#pragma omp parallel for collapse(2)
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            // Use stack variables instead of 'new'
            double exp_x = 0.0;
            double exp_y = 0.0;

            double pre_exp_x = (x - half_w) * inv_w;
            double pre_exp_y = (y - half_h) * inv_h;

            // Pass the address of your stack variables
            complexLog(&exp_y, &exp_x, pre_exp_x, pre_exp_y, real, imag, 0, 0);

            // Re-map the coordinates
            int src_x = (exp_x + half_w) * width;
            int src_y = static_cast<int>((exp_y + half_h) * height);

            src_x = (src_x % width);
            src_y = (src_y % height);

            frame(x, y) = pre_exp_frame(clamp(src_x, 0, width -1), clamp(src_y, 0, height - 1));
        }
    }
}

Log::Log(std::string arg)
{
    auto args = strip_string_vector(split_string(arg, ARG_SEPARATOR));

    if (args.size() >= 1)
    {
        if (is_str_num(args[0]))
            real = std::stod(args[0]);
        else if (args[0] == "e")
            real = E;
    }
    
    if (args.size() >= 2 and is_str_num(args[1]))
    {
        imag = std::stod(args[1]);
    }

    if (args.size() >= 3 and is_str_num(args[2]))
    {
        w_offset = std::stod(args[2]);
    }

    if (args.size() >= 4 and is_str_num(args[3]))
    {
        h_offset = std::stod(args[3]);
    }

    if (args.size() >= 5 and is_str_num(args[4]))
    {
        scale = std::stod(args[4]);
    }
}

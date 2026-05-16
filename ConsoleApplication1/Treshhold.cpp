#include "Treshhold.h"

#include <string>

#include "Process.h"
#include "Color.h"
#include "initialization.h"

void Treshhold::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    if (treshhold >= 0)
    {
#pragma omp parallel for
        for (int i = 0; i < frame.size(); i++) {

            Color& curr_col = frame(i);
            unsigned char gray = (curr_col.r + curr_col.b + curr_col.g) / 3;

            if (gray < treshhold) {
                curr_col = { 0,0,0 };
            }
        }
    }
    else
    {
        treshhold = -treshhold;
#pragma omp parallel for
        for (int i = 0; i < frame.size(); i++) {

            Color& curr_col = frame(i);
            unsigned char gray = (curr_col.r + curr_col.b + curr_col.g) / 3;

            if (gray > treshhold) {
                curr_col = { 255,255,255 };
            }
        }
    }
}


Treshhold::Treshhold(std::string arg)
{
    auto argument = strip_string(arg);
    if (is_str_int(argument))
        treshhold = std::stoi(argument);
    else if (is_str_float(argument))
        treshhold = 255 * std::stof(argument);
}

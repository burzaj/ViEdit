#include "Start.h"

#include "initialization.h"

void Start::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    if (!init)
    {
        stagger = process.frames * stagger;
        init = true;
    }

    if (curr_count <= stagger)
    {
        curr_count += 1;
        process.skip_frame = true;
        process.skip_effects = 2147483647;
    }
}

Start::Start(std::string arg)
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
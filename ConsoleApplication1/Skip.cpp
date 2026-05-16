#include "Skip.h"

#include "initialization.h"
#include "Macros.h"

void Skip::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    if (!init)
    {
        stagger = process.frames * stagger;
        init = true;
    }

    if (curr_count <= stagger and !reversed)
    {
        curr_count += 1;
        process.skip_effects = numb_of_effects_to_skip;
    }
    else if (curr_count > stagger and reversed)
    {
        curr_count -= 1;
    }
    else if (curr_count <= stagger and reversed)
    {
        process.skip_effects = numb_of_effects_to_skip;
    }
}

Skip::Skip(std::string arg)
{
    auto args = strip_string_vector(split_string(arg, ARG_SEPARATOR));

    curr_count = 0;
    if (args[0] != "end" and arg.size() > 0 and is_str_num(args[0]))
    {
        curr_count = 0;
        if (is_str_int(args[0]))
            stagger = std::stoi(args[0]);
        else if (is_str_float(args[0]))
        {
            stagger = std::stof(args[0]);
            init = false;
            if (stagger < 0)
                reversed = true;
        }
    }
    else
        stagger = 2147483647;
    
    if (args.size() > 1 and is_str_int(args[1]))
    {
        numb_of_effects_to_skip = std::stoi(args[1])+1;
    }
}

#include <filesystem>
#include <iostream>
#include <vector>
#include <cstdio>
#include <string>
#include <numeric>
#include <omp.h>
#include <algorithm>
#include <thread>

#include "initialization.h"

#include "ViEditGui.h"
#include "Process.h"
#include "Effect.h"
#include "GlobalEffect.h"
#include "Macros.h"

namespace fs = std::filesystem;

#define MANUALY_STARTED 1
#define LAST_ARG argv[argc - 1]

static void on_start_init()
{
    std::filesystem::create_directory("Output");
}

static int run_throu_terminal(const std::vector<std::string>& args)
{
    std::vector<fs::path> filepaths;
    std::vector<std::vector<Effect*>> effects;
    std::vector<std::vector<GlobalEffect*>> global_effects;

    parse_cmd_args(filepaths, effects, global_effects, args);

    std::vector<Process> processes;

    size_t num = 0;
    for (auto filepath : filepaths)
    {
        processes.push_back({ filepath, effects[0], global_effects[0], num });

        auto passed_filepath = processes.back().peek_output();
        num++;

        for (size_t i = 1; i < effects.size(); i++)
        {
            processes.push_back({ passed_filepath, effects[i], global_effects[i], num});
            passed_filepath = processes.back().peek_output();

            num++;
        }
    }

    processes.reserve(filepaths.size());

    int file_count = filepaths.size();
    int process_count = processes.size() / file_count;
    int curr_proc = 1;
    int curr_file = 1;
    for (auto& proc : processes)
    {
        std::cout << proc.start(true, curr_proc, process_count, curr_file, file_count) << "\n";
        curr_proc++;
        if (curr_proc > process_count)
        {
            curr_proc = 0;
            curr_file++;
        }
    }

    for (auto& effects_set : effects)
    {
        for (auto eff : effects_set)
        {
            delete eff;
        }
    }

    for (auto& effects_set : global_effects)
    {
        for (auto eff : effects_set)
        {
            delete eff;
        }
    }

    return 0;
}

static int gui_mode(std::vector<fs::path> path = std::vector<fs::path>())
{
    ViEditGui gui(path);

    return gui.start();
}

int main(int argc, char* argv[]) 
{
    on_start_init();

    //i wish that i could make this a function

    std::vector<std::string> args;
    for (int i = 1; i < argc; i++)
        args.push_back(argv[i]);


    if (argc == MANUALY_STARTED)
    {
        return gui_mode();
    }
    else if (is_path(args.back())) //dropped file
    {
        std::vector<fs::path> paths;

        for (auto arg : args)
            paths.push_back(arg);

        gui_mode(paths);
    }
    else
    {
        return run_throu_terminal(args);
    }

    return 0;
}
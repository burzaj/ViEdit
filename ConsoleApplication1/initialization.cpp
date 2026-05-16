#include <filesystem>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <functional>
#include <charconv> // For std::from_chars

#include "initialization.h"
#include "GlobalEffect.h"
#include "Effect.h"

//effects
#include "AtkinsonDither.h"
#include "Treshhold.h"
#include "Palette.h"
#include "Save.h"
#include "Reset.h"
#include "MaxBrightness.h"
#include "AtkinsonColorDithering.h"
#include "GaussianBlur.h"
#include "Abort.h"
#include "Skip.h"
#include "Start.h"
#include "Rectangle.h"
#include "Text.h"
#include "Log.h"
#include "Pow.h"

//global effects
#include "Scale.h"
#include "Vertical.h"

namespace fs = std::filesystem;

#define ARG_NAME 0
#define ARG_CONTENT 1

std::string strip_string(const std::string& string, char character)
{
    auto parsable_str = string;
    for (int i = 0; i < parsable_str.size(); i++)
    {
        if (parsable_str[i] == character)
        {
            parsable_str.erase(i,1);
        }
        else 
            break;
    }

    for (int i = parsable_str.size()-1; i >= 0; i--)
    {
        if (parsable_str[i] == character)
        {
            parsable_str.erase(i,1);
        }
        else
            break;
    }

    return parsable_str;
}

std::vector<std::string> strip_string_vector(const std::vector<std::string>& string, char character)
{
    std::vector<std::string> vec_copy;
    for (auto& str : string)
    {
        vec_copy.push_back(strip_string(str, character));
    }
    return vec_copy;
}

std::vector<unsigned char> load_file_binary(const fs::path& path) {
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) throw std::exception(("E: File \"" + path.string() + "\" Could Not Be Openned").c_str());

    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<unsigned char> buffer(size);
    file.read(reinterpret_cast<char*>(buffer.data()), size);
    return buffer;
}

bool is_str_num(const std::string& s) {
    if (s.empty()) return false;

    double val;
    auto [ptr, ec] = std::from_chars(s.data(), s.data() + s.size(), val);

    // If ec is success and we consumed the whole string, it's a valid number
    return ec == std::errc{} && ptr == s.data() + s.size();
}

bool is_str_int(const std::string& string)
{
    auto splited = split_string(string, '.');

    return splited.size() == 1 and is_str_num(string);
}

bool is_str_float(const std::string& string)
{
    auto splited = split_string(string, '.');

    return splited.size() == 2 and is_str_num(string);
}

std::string get_arg(const std::string& a, const std::string& b)
{
    return a.substr(b.size()+1, a.size()-1);
}

std::vector<std::string> split_string(const std::string& to_split, const char& sep)
{
    std::vector<std::string> separated;
    std::string chunk = "";

    for (const auto& chr : to_split)
    {
        if (chr != sep)
        {
            chunk += chr;
        }
        else
        {
            separated.push_back(chunk);
            chunk = "";
        }
    }

    if (chunk != "")
        separated.push_back(chunk);

    return separated;
}

bool is_path(std::string path) {
    if (path.size() < 4) return false;

    fs::path fs_path = path;

    if (!fs::exists(fs_path))
        return false;

    // find the last dot
    auto ext = fs_path.extension();

    // Simple manual lowercase check or direct comparison
    return (ext == ".jpg" || ext == ".png" || ext == ".bmp" || ext == ".mp4" ||
        ext == ".JPG" || ext == ".PNG" || ext == ".BMP" || ext == ".MP4");
}

std::string clean_path(std::string path) {
    if (path.empty()) return path;
    if ((path.front() == '"' && path.back() == '"') ||
        (path.front() == '\'' && path.back() == '\'')) {
        return path.substr(1, path.size() - 2);
    }
    return path;
}

static void parse_argument(std::vector<std::vector<Effect*>>& effects,
    std::vector<std::vector<GlobalEffect*>>& global_effects,
    bool& non_global_found,
    size_t& curr_process,
    std::string arg)
{
    static const std::unordered_map<std::string, std::function<Effect* (std::string)>> effects_map = {
    {"atk_dither" ,[](std::string arg) { return new AtkinsonDithering(arg);}},
    {"treshhold" , [](std::string arg) {return new Treshhold(arg);}},
    {"save" , [](std::string arg) {return new Save(arg);}},
    { "palette" , [](std::string arg) {return new Palette(arg);} },
    { "reset" , [](std::string arg) {return new Reset(arg);} },
    {"norm_bright",[](std::string arg) { return new MaxBrightness(arg); }},
    {"atk_col_dither",[](std::string arg) { return new AtkinsonColorDithering(arg); }},
    {"gauss_blur", [](std::string arg) { return new GaussianBlur(arg);}},
    {"abort", [](std::string arg) { return new Abort(arg);}},
    {"skip", [](std::string arg) { return new Skip(arg);}},
    {"start", [](std::string arg) { return new Start(arg);}},
    {"rect", [](std::string arg) { return new Rectangle(arg);}},
    {"text", [](std::string arg) { return new Text(arg);}},
    {"log", [](std::string arg) { return new Log(arg);}},
    {"pow", [](std::string arg) { return new Pow(arg);}}
    };

    static const std::unordered_map<std::string, std::function<GlobalEffect* (std::string)>> global_effects_map = {
        {"scale" ,[](std::string arg) { return new Scale(arg);}},
        {"vertical" ,[](std::string arg) { return new Vertical(arg);}}
    };

    if (arg[arg.size() - 1] == ')')
        arg.pop_back();

    auto separated = split_string(arg, '(');
    if (separated.size() == 1)
        separated.push_back("null");

    if (effects_map.count(separated[ARG_NAME]) == 1) {
        Effect* effect = effects_map.at(separated[ARG_NAME])(separated[ARG_CONTENT]);
        effects[curr_process].push_back(effect);
        non_global_found = true;
    }
    else if (global_effects_map.count(separated[ARG_NAME]) == 1) {

        if (non_global_found)
        {
            curr_process++;
            global_effects.push_back(std::vector<GlobalEffect*>());
            effects.push_back(std::vector<Effect*>());
        }

        non_global_found = false;

        GlobalEffect* effect = global_effects_map.at(separated[ARG_NAME])(separated[ARG_CONTENT]);
        global_effects[curr_process].push_back(effect);
    }
}

static void load_effects_from_file(
    std::vector<std::vector<Effect*>>& effects,
    std::vector<std::vector<GlobalEffect*>>& global_effects,
    fs::path filepath)
{
    std::ifstream txt(filepath);

    std::string line;

    bool non_global_found = false;
    size_t curr_process = 0;

    while (std::getline(txt, line))
    {
        line = strip_string(line, ' ');
        line = strip_string(line, ' ');

        if (line == "fnproc")
        {
            curr_process++;
        }

        parse_argument(effects, global_effects, non_global_found, curr_process, line);
    }
}

void parse_cmd_args(std::vector<fs::path>& filepaths, 
    std::vector<std::vector<Effect*>>& effects, 
    std::vector<std::vector<GlobalEffect*>>& global_effects, 
    std::vector<std::string> args)
{
   effects.resize(1);
   global_effects.resize(1);

   bool non_global_found = false;
   size_t curr_process = 0;

   for (auto arg : args)
   {
       arg = clean_path(arg);
       arg = strip_string(arg);

       if (arg == "fnproc")
       {
           curr_process++;
       }
       else if (fs::path(arg).extension().string() == ".txt")
       {
           load_effects_from_file(effects, global_effects, clean_path(arg));
       }
       else if (is_path(arg))
       {
           filepaths.push_back(clean_path(arg));
       }
       else
       {
           parse_argument(effects, global_effects, non_global_found, curr_process, arg);
       }
   }
}

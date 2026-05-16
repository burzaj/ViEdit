#include "Palette.h"

#include <vector>
#include <fstream>
#include <filesystem>

#include "initialization.h"
#include "Color.h"
#include "Macros.h"

static void read_palette_from_hex(fs::path filename, std::vector<Color>& palette)
{
    std::ifstream infile(filename);
    std::string line;
    while (std::getline(infile, line))
    {
        if (line.size() > 6)
        {
            throw std::exception("E: Bad File Format");
            break;
        }
        unsigned char col[3] = { 0,0,0 };
        for (int i = 0; i < 3; i++)
        {
            std::string str_channel = std::string(1, line[i * 2]) + std::string(1, line[i * 2 + 1]);
            col[i] = static_cast<unsigned char>(std::stoul(str_channel, nullptr, 16));
        }
        palette.push_back({ col[0], col[1], col[2] });
    }
}

static void load_from_known(fs::path filename, std::vector<Color>& palette)
{
    std::filesystem::create_directory(paths::palettes_dir);

    read_palette_from_hex(paths::palettes_dir / fs::path(filename.string() + paths::palette_extension.string()), palette);

}

static void load_from_unknown(fs::path filepath, std::vector<Color>& palette)
{
    fs::path extension = filepath.extension();

    if (extension == paths::palette_extension)
    {
        read_palette_from_hex(filepath, palette);
    }
    else
    {
        throw std::exception("E: Bad File Format");
    }
}

static void load_palette_from_args(std::vector<std::string> separated, std::vector<Color>& palette)
{
    unsigned char col[3] = { 0,0,0 };
    int iter = 0;

    for (auto& channel : separated)
    {
        if (iter < 3)
        {
            std::cout << channel << "\n";
            col[iter] = std::stoi(channel);
            iter++;
        }
        else
        {
            palette.push_back({ col[0], col[1], col[2] });
            std::cout << (int)col[0] << ", " << (int)col[1] << ", " << (int)col[2] << "\n";
            col[0] = col[1] = col[2] = 0;
            iter = 0;
            col[iter] = std::stoi(channel);
            iter++;
        }
    }
    palette.push_back({ col[0], col[1], col[2] });
}

void calculate_palette_lookup(std::vector<Color>& palette, 
    size_t palette_lookup[LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION][LOOKUP_PALLETE_RESOLUTION])
{
    float step = 255.0f / (LOOKUP_PALLETE_RESOLUTION - 1);

    for (int r = 0; r < LOOKUP_PALLETE_RESOLUTION; r++) {
        for (int g = 0; g < LOOKUP_PALLETE_RESOLUTION; g++) {
            for (int b = 0; b < LOOKUP_PALLETE_RESOLUTION; b++) {

                // Calculate the actual RGB center for this LUT bin
                float target_r = r * step;
                float target_g = g * step;
                float target_b = b * step;

                int best_index = 0;
                double min_distance = 1e10;

                for (int i = 0; i < (int)palette.size(); ++i) {
                    // Standard Euclidean Distance (No Normalization)
                    float dr = target_r - palette[i].r;
                    float dg = target_g - palette[i].g;
                    float db = target_b - palette[i].b;

                    double distance = (dr * dr) + (dg * dg) + (db * db);

                    if (distance < min_distance) {
                        min_distance = distance;
                        best_index = i;
                    }
                }

                palette_lookup[r][g][b] = best_index;
            }
        }
    }
}

void Palette::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    if (palette.empty()) return; // Safety check

#pragma omp parallel for
    for (int i = 0; i < frame.size(); ++i) {

        Color& curr_col = frame(i);
        curr_col /= divisor;

        // Clamp indices just in case of rounding/edge cases
        if (curr_col.r >= LOOKUP_PALLETE_RESOLUTION) curr_col.r = LOOKUP_PALLETE_RESOLUTION - 1;
        if (curr_col.g >= LOOKUP_PALLETE_RESOLUTION) curr_col.g = LOOKUP_PALLETE_RESOLUTION - 1;
        if (curr_col.b >= LOOKUP_PALLETE_RESOLUTION) curr_col.b = LOOKUP_PALLETE_RESOLUTION - 1;

        // 2. Get index from LUT
        int index = palette_lookup[curr_col.r][curr_col.g][curr_col.b];

        // 3. Final safety check for the palette vector
        if (index >= 0 && index < palette.size()) {
            Color col = palette[index];
            curr_col = col;
        }
    }
}

Palette::Palette(std::string args)
{
    divisor = 255 / LOOKUP_PALLETE_RESOLUTION;

    std::vector<std::string> separated = strip_string_vector(split_string(args, ARG_SEPARATOR));

    std::cout << separated[separated.size()-1] << "\n";

    if (separated.size() > 1)
    {
        load_palette_from_args(separated, palette);
    }
    else if (is_path(separated[0]))
    {
        load_from_unknown(separated[0], palette);
    }
    else
    {
        load_from_known(separated[0], palette);
    }

    calculate_palette_lookup(palette, palette_lookup);
}

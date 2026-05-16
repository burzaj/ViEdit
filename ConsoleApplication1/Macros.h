#pragma once

#include <filesystem>

#define ARG_SEPARATOR ','

namespace paths {
    namespace fs = std::filesystem;

    inline const fs::path output_dir = "Output";
    inline const fs::path fonts_dir =  "Fonts";
    inline const fs::path palettes_dir = "Palettes";

    inline const fs::path gui_font = "ARIAL";
    inline const fs::path font_extension = ".ttf";
    inline const fs::path palette_extension = ".hex";
}
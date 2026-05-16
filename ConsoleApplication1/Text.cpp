#include "Text.h"

#include <vector>
#include <fstream>
#include <filesystem>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include "Math.h"
#include "Macros.h"
#include "initialization.h"

static void load_from_known(fs::path filename, std::vector<unsigned char>& font_buff)
{
	std::filesystem::create_directory("Fonts");

	font_buff = load_file_binary(paths::fonts_dir / (filename.replace_extension(paths::font_extension)));
}

static void load_from_unknown(fs::path filepath, std::vector<unsigned char>& font_buff)
{
	font_buff = load_file_binary(filepath.replace_extension(paths::font_extension));
}

static float calculate_text_width(const std::string& text, const stbtt_fontinfo& font, float scale) {
    int total_advance = 0;

    for (int i = 0; i < text.length(); ++i) {
        int advance, leftSideBearing;
        stbtt_GetCodepointHMetrics(&font, text[i], &advance, &leftSideBearing);

        total_advance += advance;

        if (i < text.length() - 1) {
            total_advance += stbtt_GetCodepointKernAdvance(&font, text[i], text[i + 1]);
        }
    }

    return total_advance * scale;
}

void Text::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    if (!init)
    {
        if (x_scaled)
            x = clamp(width * x,0 , width);
        if (y_scaled)
            y = clamp(height * y, 0, height);

        init = true;
    }

    int cursor_x = (x - (h_align_centered ? half_text_width : 0));
    int baseline = (y + (v_align_centered ? font_ascent * font_scale / 2 : font_ascent * font_scale));

    for (char c : text) {
        if (glyph_cache.find(c) == glyph_cache.end()) continue;

        const auto& g = glyph_cache[c];

        if (!g.bitmap.empty()) {
            for (int row = 0; row < g.height; ++row) {
                for (int col = 0; col < g.width; ++col) {
                    int px = cursor_x + g.x_offset + col;
                    int py = baseline + g.y_offset + row;

                    if (g.bitmap[row * g.width + col] > 0 and (px < width and px > 0) and (py < height and py > 0)) {
                        frame(px, py) = color;
                    }
                }
            }
        }
        cursor_x += g.advance;
    }
}

Text::Text(std::string arg)
{
    auto args = split_string(arg, ARG_SEPARATOR);

    int amout_of_args = args.size();

    float size = 15.0f;

    std::vector<unsigned char> font_buffer;

    if (amout_of_args >= 4)
    {
        if (is_path(args[0]))
        {
            load_from_unknown(args[0], font_buffer);
        }
        else
        {
            load_from_known(args[0], font_buffer);
        }

        if (is_str_num(args[1]) and is_str_num(args[2]))
        {
            x = std::stof(args[1]);
            y = std::stof(args[2]);
        }

        if (is_str_float(args[1]))
            x_scaled = true;
        if (is_str_float(args[2]))
            y_scaled = true;

        text = args[3];

        int col_channel_id = 0;

        for (int i = 4; i < amout_of_args; i++)
        {
            if (args[i] == "h_center")
                h_align_centered = true;
            else if (args[i] == "v_center")
                v_align_centered = true;
            else if (args[i][0] == 's' and args[i][1] == '=')
            {
                auto size_str = split_string(args[i], '=');
                if (size_str.size() == 2 and is_str_num(size_str[1]))
                {
                    size = std::stof(size_str[1]);
                }
            }
			else if (is_str_int(args[i]))
			{
				switch (col_channel_id)
				{
				case 0:
					color.r = std::stoi(args[i]);
					break;
				case 1:
					color.g = std::stoi(args[i]);
					break;
				case 2:
					color.b = std::stoi(args[i]);
					break;
				}
				col_channel_id++;
			}
		}
	}

    {
        stbtt_fontinfo font;
        stbtt_InitFont(&font, font_buffer.data(), 0);

        font_scale = stbtt_ScaleForPixelHeight(&font, size);
        int d, g;
        stbtt_GetFontVMetrics(&font, &font_ascent, &d, &g);

        for (unsigned char c = 32; c < 128; ++c) {
            int x0, y0, x1, y1, adv, lsb;
            stbtt_GetCodepointBitmapBox(&font, c, font_scale, font_scale, &x0, &y0, &x1, &y1);
            stbtt_GetCodepointHMetrics(&font, c, &adv, &lsb);

            int w = x1 - x0;
            int h = y1 - y0;

            CachedGlyph glyph;
            glyph.width = w;
            glyph.height = h;
            glyph.x_offset = x0;
            glyph.y_offset = y0;
            glyph.advance = static_cast<int>(adv * font_scale);

            if (w > 0 && h > 0) {
                glyph.bitmap.resize(w * h);
                stbtt_MakeCodepointBitmap(&font, glyph.bitmap.data(), w, h, w, font_scale, font_scale, c);
            }
            glyph_cache[c] = glyph;
        }
        half_text_width = calculate_text_width(text, font, font_scale) / 2.0f;
    }
}

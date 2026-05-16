#include "AtkinsonColorDithering.h"

#include "Palette.h"
#include "Math.h"
#include "Process.h"

static void distribute_color_error(int x, int y, float errR, float errG, float errB, std::vector<float>& atkinson_flloat, const size_t& width, const size_t& height) {
    // 1. SprawdŸ, czy piksel mieœci siê w granicach obrazu
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    // 2. Oblicz indeks (zak³adaj¹c uk³ad RGB 24-bit)
    int idx = (x + y * width) * 3;

    // 3. Pobierz, zmodyfikuj i ogranicz (clamp) wartoœci dla ka¿dego kana³u
    // Kana³ R
    float r = atkinson_flloat[idx] + errR;
    atkinson_flloat[idx] = (r < 0 ? 0 : (r > 255 ? 255 : r));

    // Kana³ G
    float g = atkinson_flloat[idx + 1] + errG;
    atkinson_flloat[idx + 1] = (g < 0 ? 0 : (g > 255 ? 255 : g));

    // Kana³ B
    float b = atkinson_flloat[idx + 2] + errB;
    atkinson_flloat[idx + 2] = (b < 0 ? 0 : (b > 255 ? 255 : b));
}

void AtkinsonColorDithering::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    const int divisor = 256 / LOOKUP_PALLETE_RESOLUTION;

    std::vector<float> atkinson_flloat(frame.size()*3);
    for (int i = 0; i < frame.size() * 3; ++i) {
        atkinson_flloat[i] = frame[i];
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            size_t idx = (x + y * width) * 3;

            // 1. ZnajdŸ najbli¿szy kolor z palety
            Color new_pixel(clamp(atkinson_flloat[idx] / divisor, 0, LOOKUP_PALLETE_RESOLUTION - 1),
                clamp(atkinson_flloat[idx + 1] / divisor, 0, LOOKUP_PALLETE_RESOLUTION - 1),
                clamp(atkinson_flloat[idx + 2] / divisor, 0, LOOKUP_PALLETE_RESOLUTION - 1));

            new_pixel *= divisor;

            // 2. Oblicz b³¹d dla ka¿dego kana³u
            float errR = clamp((atkinson_flloat[idx] - new_pixel.r) / 8.0f, -255, 255);
            float errG = clamp((atkinson_flloat[idx + 1] - new_pixel.g) / 8.0f, -255, 255);
            float errB = clamp((atkinson_flloat[idx + 2] - new_pixel.b) / 8.0f, -255, 255);

            frame(x, y) = new_pixel;

            // 3. Rozprosz b³¹d (Atkinson pattern)
            distribute_color_error(x + 1, y, errR, errG, errB, atkinson_flloat, width, height);
            distribute_color_error(x + 2, y, errR, errG, errB, atkinson_flloat, width, height);
            distribute_color_error(x - 1, y + 1, errR, errG, errB, atkinson_flloat, width, height);
            distribute_color_error(x, y + 1, errR, errG, errB, atkinson_flloat, width, height);
            distribute_color_error(x + 1, y + 1, errR, errG, errB, atkinson_flloat, width, height);
            distribute_color_error(x, y + 2, errR, errG, errB, atkinson_flloat, width, height);
        }
    }
}

AtkinsonColorDithering::AtkinsonColorDithering(std::string arg)
{
}

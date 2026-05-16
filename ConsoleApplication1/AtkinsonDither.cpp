#include "AtkinsonDither.h"

#include "Color.h"

#include "Process.h"

void distribute_error(Frame& frame, const size_t& x, const size_t& y, float error)
{
    if (x < 0 || x >= frame.width() || y < 0 || y >= frame.height()) return;
    frame(x, y) += error;
    return;
}

void AtkinsonDithering::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Poprawiony indeks (x + y * width)
            int idx = (x + y * width);

            Color pixel_col = frame(idx);

            float gray = (pixel_col.r + pixel_col.g + pixel_col.b)/3.0f;

            unsigned char output = gray > 127 ? 255 : 0;

            float error = ((gray - output) / 8.0f);

            distribute_error(frame,x + 1, y,     error);
            distribute_error(frame,x + 2, y,     error);
            distribute_error(frame,x - 1, y + 1, error);
            distribute_error(frame,x,     y + 1, error);
            distribute_error(frame,x + 1, y + 1, error);
            distribute_error(frame,x,     y + 2,     error);
        }
    }
}

AtkinsonDithering::AtkinsonDithering(std::string arg)
{
}

#include "Effects.h"
/*
#include <vector>
#include <string>
#include <iostream>
#include <omp.h>

#include "Macros.h"
#include "Math.h"
#include "FrameProcessing.h"

void bayer_dither()
{
    mean = get_frame_mean(frame, mean);
#pragma omp parallel for
    for (int pixel_idx = 0; pixel_idx < frame_size / 3; ++pixel_idx) {
        int i = pixel_idx * 3;
        int x = pixel_idx % width;
        int y = pixel_idx / width;

        // Standard Grayscale conversion
        unsigned char _threshold = bayer_matrix[y % M_size][x % M_size];

        int bw = (unsigned char)(_threshold)-mean*mean_shift;

        // Apply threshold
        frame[i] = clamp(frame[i] + bw, 0, 255);
        frame[i + 1] = clamp(frame[i+1] + bw, 0, 255);
        frame[i + 2] = clamp(frame[i+2] + bw, 0, 255);
    }
}

#define maxval(x,y,z, max_channel) x > y and x > z ? x : max_channel



void bayer_treshhold()
{
    mean = get_frame_mean(frame, mean);
#pragma omp parallel for
    for (int pixel_idx = 0; pixel_idx < frame_size / 3; ++pixel_idx) {
        int i = pixel_idx * 3;
        int x = pixel_idx % width;
        int y = pixel_idx / width;

        unsigned char r = original_frame[i];
        unsigned char g = original_frame[i + 1];
        unsigned char b = original_frame[i + 2];

        unsigned char gray = (r * redstrength + b * bluestrength + g * greenstrength) / 3;

        // Standard Grayscale conversion
        unsigned char _threshold = bayer_matrix[y % M_size][x % M_size];

        int bw = (unsigned char)(_threshold)-mean * mean_shift;

        // Apply threshold
        if (gray > bw)
        { 
            frame[i] = 0;
            frame[i + 1] = 0;
            frame[i + 2] = 0;
        }
    }
}

void apply_multiplyer()
{
#pragma omp parallel for
    for (int pixel_idx = 0; pixel_idx < frame_size / 3; ++pixel_idx) {
        int i = pixel_idx * 3;

            frame[i] *= multiplayer;
            frame[i + 1] *= multiplayer;
            frame[i + 2] *= multiplayer;
    }
}

void max_brightnes_colors()
{

#pragma omp parallel for
    for (int i = 0; i < frame_size / 3; ++i) {
        int base = i * 3;
        if (base + 2 >= frame_size) continue;

        float r = frame[base] * redstrength;
        float g = frame[base + 1] * greenstrength;
        float b = frame[base + 2] * bluestrength;

        // 1. Find the highest value among the three
        float max_ch = std::max({ r, g, b });

        // 2. Only scale if there is actual color (prevents NaN)
        // and only scale if the pixel isn't already at max brightness
        if (max_ch > 0.0f) {
            float scale = 255.0f / max_ch;
            frame[base] = r * scale;
            frame[base + 1] = g * scale;
            frame[base + 2] = b * scale;
        }
    }
}

void color_pallete()
{
    if (palette.empty()) return; // Safety check

    // Calculate how much to "shrink" the color value to fit the LUT size
    // Example: If map_pallete is 32, we need to divide by 8 (256/32)
    // Using a bit-shift is faster: 256/32 = 8, and 8 is 2^3, so we shift by 3.
    const int divisor = 256 / map_pallete;

#pragma omp parallel for
    for (int i = 0; i < width * height; ++i) {
        int base = i * 3;

        // Ensure we are within the frame vector bounds
        if (base + 2 >= frame.size()) continue;

        int debug = frame[base];

        unsigned char r = frame[base] / divisor * redstrength;
        unsigned char g = frame[base + 1] / divisor * greenstrength;
        unsigned char b = frame[base + 2] / divisor * bluestrength;

        // 1. Correct LUT indexing
        int lut_r = r;
        int lut_g = g;
        int lut_b = b;

        // Clamp indices just in case of rounding/edge cases
        if (lut_r >= map_pallete) lut_r = map_pallete - 1;
        if (lut_g >= map_pallete) lut_g = map_pallete - 1;
        if (lut_b >= map_pallete) lut_b = map_pallete - 1;

        // 2. Get index from LUT
        int index = palette_lookup[lut_r][lut_g][lut_b];

        // 3. Final safety check for the palette vector
        if (index >= 0 && index < palette.size()) {
            frame[base] = palette[index].r;
            frame[base + 1] = palette[index].g;
            frame[base + 2] = palette[index].b;
        }
    }
}

void outline() {
#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int curr_idx = y * width + x;
            bool is_edge = false;

            // Search within the 'outline_size' radius
            for (int ky = -outline_size; ky <= outline_size; ++ky) {
                for (int kx = -outline_size; kx <= outline_size; ++kx) {
                    int ny = y + ky;
                    int nx = x + kx;

                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        int neighbor_idx = ny * width + nx;

                        // Compare current pixel to neighbor
                        if (get_dist(curr_idx, neighbor_idx) > outline_strength) {
                            is_edge = true;
                            break;
                        }
                    }
                }
                if (is_edge) break;
            }

            if (is_edge) {
                int base = curr_idx * 3;
                frame[base] = 0;     // R (Black outline)
                frame[base + 1] = 0;   // G
                frame[base + 2] = 0;   // B
            }
        }
    }
}

void outline_for_palette() {
    const int divisor = 256 / map_pallete;
    // We read from this to avoid race conditions
    const std::vector<unsigned char> original = frame;

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int curr_base = (y * width + x) * 3;

            // 1. Move current pixel lookup OUTSIDE the kernel loop
            unsigned char cr = original[curr_base] / divisor;
            unsigned char cg = original[curr_base + 1] / divisor;
            unsigned char cb = original[curr_base + 2] / divisor;

            // Note: Add safety check for LUT bounds if map_pallete is small
            int current_lut_id = palette_lookup[cr][cg][cb];

            bool is_edge = false;

            // 2. Search neighbors
            for (int ky = -outline_size; ky <= outline_size; ++ky) {
                for (int kx = -outline_size; kx <= outline_size; ++kx) {
                    if (kx == 0 && ky == 0) continue; // Skip self

                    int ny = y + ky;
                    int nx = x + kx;

                    // 3. Use your fixed int-based clamp/check for coordinates
                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        int neighbor_base = (ny * width + nx) * 3;

                        // READ FROM 'original', NOT 'frame'
                        unsigned char nr = original[neighbor_base] / divisor;
                        unsigned char ng = original[neighbor_base + 1] / divisor;
                        unsigned char nb = original[neighbor_base + 2] / divisor;

                        int neighbor_lut_id = palette_lookup[nr][ng][nb];

                        // 4. Compare IDs directly (Fastest way)
                        if (neighbor_lut_id != current_lut_id) {
                            is_edge = true;
                            break;
                        }
                    }
                }
                if (is_edge) break;
            }

            if (is_edge) {
                frame[curr_base] = 0; // Write to the output frame
                frame[curr_base + 1] = 0;
                frame[curr_base + 2] = 0;
            }
        }
    }
}

void remove_noise() {
    const int divisor = 256 / map_pallete;
    const std::vector<unsigned char> noise_frame = frame; // Fixed snapshot

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int curr_base = (y * width + x) * 3;

            // Map to count how many times each Palette ID appears
            // Using a small fixed array is faster than std::map if palette is small
            std::vector<int> counts(palette.size(), 0);

            int my_r = noise_frame[curr_base] / divisor;
            int my_g = noise_frame[curr_base + 1] / divisor;
            int my_b = noise_frame[curr_base + 2] / divisor;
            int my_id = palette_lookup[my_r][my_g][my_b];

            for (int ky = -denoise_grain; ky <= denoise_grain; ++ky) {
                for (int kx = -denoise_grain; kx <= denoise_grain; ++kx) {
                    int ny = y + ky;
                    int nx = x + kx;

                    if (ny >= 0 && ny < height && nx >= 0 && nx < width) {
                        int n_base = (ny * width + nx) * 3;
                        int nr = noise_frame[n_base] / divisor;
                        int ng = noise_frame[n_base + 1] / divisor;
                        int nb = noise_frame[n_base + 2] / divisor;
                        counts[palette_lookup[nr][ng][nb]]++;
                    }
                }
            }

            // Find the most frequent color ID in the neighborhood
            int max_votes = 0;
            int winner_id = my_id;
            for (int i = 0; i < counts.size(); i++) {
                if (counts[i] > max_votes) {
                    max_votes = counts[i];
                    winner_id = i;
                }
            }

            // ONLY change if the winner is significantly more popular than the current color
            // This prevents "flickering" or "flipping"
            float win_percent = (float)max_votes / (float)((denoise_grain * 2 + 1) * (denoise_grain * 2 + 1));

            if (winner_id != my_id && win_percent > denoise_strength) {
                frame[curr_base] = palette[winner_id].r;
                frame[curr_base + 1] = palette[winner_id].g;
                frame[curr_base + 2] = palette[winner_id].b;
            }
        }
    }
}



#define getidx(x,y) (x + y * width)*3

void atkinson_dithering() {

}

inline Color get_color_from_work_buffer(int x, int y, const std::vector<float>& buf) {
    // Bezpiecznik: jeœli wyjdziemy poza obraz, zwróæ czarny (lub obs³u¿ b³¹d)
    if (x < 0 || x >= width || y < 0 || y >= height) return { 0.0f, 0.0f, 0.0f };

    int idx = (x + y * width) * 3;
    return {
        (clampf(buf[idx], 0.0f, 255.0f)),
        (clampf(buf[idx + 1], 0.0f, 255.0f)),
        (clampf(buf[idx + 2], 0.0f, 255.0f))
    };
}

void atkinson_color_dithering() {
    const int divisor = 256 / map_pallete;

    std::vector<float> atkinson_flloat(frame_size);
    for (int i = 0; i < frame_size; ++i) {
        atkinson_flloat[i] = static_cast<float>(frame[i]);
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Color oldPixel = get_color_from_work_buffer(x,y, atkinson_flloat);

            // 1. ZnajdŸ najbli¿szy kolor z palety
            int r_idx = clamp(oldPixel.r / divisor, 0, map_pallete - 1);
            int g_idx = clamp(oldPixel.g / divisor, 0, map_pallete - 1);
            int b_idx = clamp(oldPixel.b / divisor, 0, map_pallete - 1);

            Color newPixel = palette[palette_lookup[r_idx][g_idx][b_idx]];

            // 2. Oblicz b³¹d dla ka¿dego kana³u
            float errR = clamp((oldPixel.r - newPixel.r) / 8.0f, -255, 255);
            float errG = clamp((oldPixel.g - newPixel.g) / 8.0f, -255, 255);
            float errB = clamp((oldPixel.b - newPixel.b) / 8.0f, -255, 255);

            int idx = getidx(x,y);
            frame[idx] = newPixel.r;
            frame[idx+1] = newPixel.g;
            frame[idx+2] = newPixel.b;

            // 3. Rozprosz b³¹d (Atkinson pattern)
            distribute_color_error(x + 1, y, errR, errG, errB, atkinson_flloat);
            distribute_color_error(x + 2, y, errR, errG, errB, atkinson_flloat);
            distribute_color_error(x - 1, y + 1, errR, errG, errB, atkinson_flloat);
            distribute_color_error(x, y + 1, errR, errG, errB, atkinson_flloat);
            distribute_color_error(x + 1, y + 1, errR, errG, errB, atkinson_flloat);
            distribute_color_error(x, y + 2, errR, errG, errB, atkinson_flloat);
        }
    }
}*/
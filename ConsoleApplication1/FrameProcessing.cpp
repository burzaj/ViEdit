#include "FrameProcessing.h"

#include "Macros.h"
#include "Math.h"
/*
void distribute_color_error(int x, int y, float errR, float errG, float errB, std::vector<float>& atkinson_flloat){
    // 1. SprawdŸ, czy piksel mieœci siê w granicach obrazu
    if (x < 0 || x >= width || y < 0 || y >= height) return;

    // 2. Oblicz indeks (zak³adaj¹c uk³ad RGB 24-bit)
    int idx = (x + y * width) * 3;

    // 3. Pobierz, zmodyfikuj i ogranicz (clamp) wartoœci dla ka¿dego kana³u
    // Kana³ R
    float r = atkinson_flloat[idx] + errR;
    atkinson_flloat[idx] = (unsigned char)(r < 0 ? 0 : (r > 255 ? 255 : r));

    // Kana³ G
    float g = atkinson_flloat[idx + 1] + errG;
    atkinson_flloat[idx + 1] = (unsigned char)(g < 0 ? 0 : (g > 255 ? 255 : g));

    // Kana³ B
    float b = atkinson_flloat[idx + 2] + errB;
    atkinson_flloat[idx + 2] = (unsigned char)(b < 0 ? 0 : (b > 255 ? 255 : b));
}

std::vector<float> create_gaussian_kernel(int radius, float sigma) {
    std::vector<float> kernel(2 * radius + 1);
    float sum = 0.0f;

    for (int i = -radius; i <= radius; ++i) {
        float val = exp(-(i * i) / (2 * sigma * sigma));
        kernel[i + radius] = val;
        sum += val;
    }

    // Normalize so the image doesn't change brightness
    for (float& f : kernel) f /= sum;
    return kernel;
}

float get_dist( int i, int j) {
    int b1 = i * 3, b2 = j * 3;
    float dr = original_frame[b1] - original_frame[b2];
    float dg = original_frame[b1 + 1] - original_frame[b2 + 1];
    float db = original_frame[b1 + 2] - original_frame[b2 + 2];
    return sqrt(dr * dr + dg * dg + db * db);
}

int get_frame_mean(std::vector<unsigned char>& frame, unsigned char& prev_mean)
{
    long long total_sum = 0;

#pragma omp parallel for reduction(+:total_sum)
    for (int i = 0; i < (int)frame.size(); ++i) {
        total_sum += frame[i];
    }
    unsigned char curr_mean = static_cast<double>(total_sum) / frame.size();

    unsigned char mean = (mean_alpha * curr_mean) + ((1.0 - mean_alpha) * prev_mean);
    return mean;
}

unsigned char get_density(int block_y, int block_x)
{
    unsigned char count = 0;

    // Calculate the starting pixel coordinates for this specific Bayer block
    int start_x = block_x * M_size;
    int start_y = block_y * M_size;

    for (int j = 0; j < M_size; j++) {
        for (int i = 0; i < M_size; i++) {
            int curr_x = start_x + i;
            int curr_y = start_y + j;

            // Boundary check: ensure we don't read outside the actual image dimensions
            if (curr_x < width && curr_y < height) {
                // Correct 1D indexing for a 3-channel (RGB) frame
                int pixel_idx = (curr_y * width + curr_x) * 3;
                if (frame[pixel_idx] != 0) {
                    count++;
                }
            }
        }
    }

    return count;
}

void calculate_pallete_lookup()

*/
/*    float step = 255.0f / (map_pallete - 1);

    for (int r = 0; r < map_pallete; r++) {
        for (int g = 0; g < map_pallete; g++) {
            for (int b = 0; b < map_pallete; b++) {

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
    }*/
#include "GaussianBlur.h"

#include <cmath>
#include <numeric>

#include "initialization.h"
#include "Math.h"
#include "Process.h"

#define M_PI 3.1415926535

static std::vector<float> create_gaussian_kernel(int radius, float sigma) {
    int size = 2 * radius + 1;
    std::vector<float> kernel(size);
    float sum = 0.0f;

    // Constant part of the Gaussian formula
    float twoSigmaSq = 2.0f * sigma * sigma;
    float constant = 1.0f / (sqrt(2.0f * M_PI) * sigma);

    for (int i = -radius; i <= radius; i++) {
        float val = constant * exp(-(i * i) / twoSigmaSq);
        kernel[i + radius] = val;
        sum += val;
    }

    // Normalize: ensure all values add up to 1.0
    for (int i = 0; i < size; i++) {
        kernel[i] /= sum;
    }

    return kernel;
}

void GaussianBlur::apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process)
{
    Frame temp = frame;

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float r = 0, g = 0, b = 0;
            for (int k = -radius; k <= radius; ++k) {
                int nx = clamp(x + k, 0, width - 1);
                int idx = (y * width + nx) * 3;
                float weight = kernel[k + radius];
                r += frame[idx] * weight;
                g += frame[idx + 1] * weight;
                b += frame[idx + 2] * weight;
            }
            int out_idx = (y * width + x) * 3;
            frame(x, y) = { r,g,b };
        }
    }

#pragma omp parallel for
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float r = 0, g = 0, b = 0;
            for (int k = -radius; k <= radius; ++k) {
                int ny = clamp(y + k, 0, height - 1);
                size_t idx = (static_cast<size_t>(ny) * width + x) * 3;
                float weight = kernel[k + radius];
                r += frame[idx] * weight;
                g += frame[idx + 1] * weight;
                b += frame[idx + 2] * weight;
            }
            frame(x, y) = { r, g, b };
        }
    }
}

GaussianBlur::GaussianBlur(std::string args)
{
    sigma = 0.1;
    if (is_str_float(strip_string(args)))
        sigma = std::stof(args);
    radius = std::ceil(3 * sigma);
    kernel = create_gaussian_kernel(radius, sigma);
}

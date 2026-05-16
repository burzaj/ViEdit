#pragma once

#include <string>
#include <vector>

#include "Effect.h"
#include "Color.h"

class GaussianBlur : public Effect
{
    float sigma;
    int radius;
    std::vector<float> kernel;
public:
    void apply(Frame& frame, const Frame& original_frame, const size_t& width, const size_t& height, Process& process) override;

    GaussianBlur(std::string args);
};

#include "Frame.h"

size_t Frame::get_idx(size_t x, size_t y)
{
    return (x + y * f_width);
}

Color& Frame::operator()(const size_t& x, const size_t& y)
{
    return frame[get_idx(x, y)];
}

Color& Frame::operator()(const size_t& idx)
{
    return frame[idx];
}

unsigned char Frame::operator[](const size_t& idx)  
{  
   size_t color_idx = idx / 3;  
   size_t channel_idx = idx % 3;  

   if (channel_idx == 0)  
       return frame[color_idx].r;  
   else if (channel_idx == 1)  
       return frame[color_idx].g;  
   else  
       return frame[color_idx].b;  
}

size_t& Frame::width()
{
    return f_width;
}

size_t& Frame::height()
{
    return f_heigth;
}

size_t Frame::size()
{
    return frame.size();
}

unsigned char* Frame::data()  
{  
   return reinterpret_cast<unsigned char*>(frame.data());  
}

Frame& Frame::operator=(const std::vector<unsigned char>& other) {
    if (other.size() % 3 != 0) {
        return *this;
    }

    size_t num_colors = other.size() / 3;
    frame.clear();
    frame.reserve(num_colors);

    for (size_t i = 0; i < other.size(); i += 3) {
        frame.emplace_back(other[i], other[i + 1], other[i + 2]);
    }

    return *this;
}

void Frame::construct(const size_t& width, const size_t& height)
{
    f_heigth = height;
    f_width = width;

    frame.resize(height * width);
}

Frame::Frame()
{
    f_heigth = 0;
    f_width = 0;
}

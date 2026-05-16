#include "Color.h"
#include "Math.h"

unsigned char Color::operator[](unsigned char idx)
{
    return idx == 0 ? r : idx == 1 ? g : idx == 2 ? b : 0;
}

Color& Color::operator*=(const Color& other)
{
    r = clamp(r * other.r, 0, 255);
    g = clamp(g * other.g, 0, 255);
    b = clamp(b * other.b, 0, 255);

    return *this;
}

Color& Color::operator*=(const float& other)
{
    r = clamp(static_cast<unsigned char>(r * other), 0, 255);
    g = clamp(static_cast<unsigned char>(g * other), 0, 255);
    b = clamp(static_cast<unsigned char>(b * other), 0, 255);

    return *this;
}

Color& Color::operator*=(const int& other)
{
    r = clamp(static_cast<unsigned char>(r * other), 0, 255);
    g = clamp(static_cast<unsigned char>(g * other), 0, 255);
    b = clamp(static_cast<unsigned char>(b * other), 0, 255);

    return *this;
}

Color& Color::operator/=(const Color& other)
{
    r = clamp(r / other.r, 0, 255);
    g = clamp(g / other.g, 0, 255);
    b = clamp(b / other.b, 0, 255);

    return *this;
}

Color& Color::operator/=(const float& other)
{
    r = clamp(static_cast<unsigned char>(r / other), 0, 255);
    g = clamp(static_cast<unsigned char>(g / other), 0, 255);
    b = clamp(static_cast<unsigned char>(b / other), 0, 255);

    return *this;
}

Color& Color::operator/=(const int& other)
{
    r = clamp(static_cast<unsigned char>(r / other), 0, 255);
    g = clamp(static_cast<unsigned char>(g / other), 0, 255);
    b = clamp(static_cast<unsigned char>(b / other), 0, 255);

    return *this;
}

Color& Color::operator=(Color other)
{
    r = other.r;
    g = other.g;
    b = other.b;

    return *this;
}

Color& Color::operator=(unsigned char other)
{
    r = other;
    g = other;
    b = other;

    return *this;
}

Color& Color::operator+=(const Color& other)
{
    r = clamp((r + other.r), 0, 255);
    g = clamp((g + other.g), 0, 255);
    b = clamp((b + other.b), 0, 255);

    return *this;
}

Color& Color::operator+=(const int& other)
{
    r = clamp(static_cast<unsigned char>(r + other), 0, 255);
    g = clamp(static_cast<unsigned char>(g + other), 0, 255);
    b = clamp(static_cast<unsigned char>(b + other), 0, 255);

    return *this;
}

Color& Color::operator-=(const Color& other)
{
    r = clamp((r - other.r), 0, 255);
    g = clamp((g - other.g), 0, 255);
    b = clamp((b - other.b), 0, 255);

    return *this;
}

Color& Color::operator-=(const int& other)
{
    r = clamp(static_cast<unsigned char>(r - other), 0, 255);
    g = clamp(static_cast<unsigned char>(g - other), 0, 255);
    b = clamp(static_cast<unsigned char>(b - other), 0, 255);

    return *this;
}

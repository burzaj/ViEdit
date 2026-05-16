#pragma once

struct Color {
    unsigned char r, g, b;

    Color() : r(0), 
        g(0), 
        b(0) {};

    Color(unsigned char _r, 
        unsigned char _g, 
        unsigned char _b) :
        r(_r), 
        g(_g), 
        b(_b) {};

    Color(int _r,
        int _g,
        int _b) :
        r(_r),
        g(_g),
        b(_b) {
    };

    Color(float _r,
        float _g,
        float _b) :
        r(static_cast<unsigned char>(_r)),
        g(static_cast<unsigned char>(_g)),
        b(static_cast<unsigned char>(_b)) {
    };

    unsigned char operator[](unsigned char idx);

    Color& operator*=(const Color& other);
    Color& operator*=(const float& other);
    Color& operator*=(const int& other);

    Color& operator/=(const Color& other);
    Color& operator/=(const float& other);
    Color& operator/=(const int& other);

    Color& operator=(const Color other);
    Color& operator=(const unsigned char other);

    Color& operator+=(const Color& other);
    Color& operator+=(const int& other);

    Color& operator-=(const Color& other);
    Color& operator-=(const int& other);
};


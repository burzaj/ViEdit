#pragma once

#include <string>
#include <vector>

#include "SFML\Graphics.hpp"

//global class to hold all data for running functions
class Global
{
    std::vector<std::string> errLog;

public:
    //rendering stuff
    sf::RenderWindow& window;
    int screenWidth;
    int screenHeight;

    //font stuff
    sf::Font font;
    int font_size;

    //list of all functions and constants user created or not

    Global(sf::RenderWindow& window_);
};


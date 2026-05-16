#include "Global.h"

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include "Macros.h"

namespace fs = std::filesystem;

Global::Global(sf::RenderWindow& window_)
   : window(window_)
{
   screenWidth = window.getView().getSize().x;
   screenHeight = window.getView().getSize().y;

   font = sf::Font();
   if (!font.openFromFile(paths::fonts_dir / (paths::gui_font.string() + paths::font_extension.string()))) {
       // Handle font loading error
   }
   font_size = 30;
}
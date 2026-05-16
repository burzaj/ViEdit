#pragma once

#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#include "SFML/Graphics.hpp"

#include "Global.h"
#include "Input.h"
#include "initialization.h"
#include "Process.h"
#include "Effect.h"
#include "GlobalEffect.h"
#include "EffectWrapper.h"

namespace gui
{
	namespace left_panel
	{
		inline const float left_panel_width = 0.2f;

		inline const float nameplate_height_text = 0.1f;
		inline const float nameplate_marigins = 0.1f;
	}
}

namespace fs = std::filesystem;

class ViEditGui
{
	sf::RenderWindow window = sf::RenderWindow(sf::VideoMode({ 1500, 1000 }, 32), "ViEdit ;)");
	Global global = Global(window);
	Input input = Input(global, [this](size_t event) {this->event(event); return;});

	std::vector<fs::path> files_selected_for_edditing = std::vector<fs::path>();

	//std::vector<GuiEffect> gui_effects;



public:
	int start();

	void event(size_t event);

	ViEditGui() {};
	ViEditGui(std::vector<fs::path> paths) : files_selected_for_edditing(paths) {};
};

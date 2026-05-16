#include "ViEditGui.h"

int ViEditGui::start()
{
	while (window.isOpen())
	{
		input.Handle_event();


		window.clear();
		window.display();
	}

	while (window.isOpen())
	{
		input.Handle_event();


		window.clear();
		window.display();
	}

	return 0;
}

void ViEditGui::event(size_t event)
{
/*
	static const size_t mb_click_ev = RELEASED + (MOUSE_LEFT << 4);
	if (event == mb_click_ev)
		for (auto& gui_effect : gui_effects)
			gui_effect.clicked_on_screen(input);
*/
}

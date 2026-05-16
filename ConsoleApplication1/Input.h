#pragma once

#include <functional>

#include "SFML\Graphics.hpp"

#include "Global.h"

enum Key
{
	NO_INPUT,
	ESC,
	TAB,
	CAPSLOCK,
	SHIFT,
	CONTROL,
	ALT,
	ENTER,
	BACKSPACE,
	DELETE,
	PAGE_UP,
	PAGE_DOWN,
	MOUSE_LEFT,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	KEY_ENUM_SIZE
};

enum KeyState
{
	DOWN,
	UP,
	PRESSED,
	RELEASED,
	CHARPRESSED,
	MOUSEWHEEL,
	MOUSEMOVED
};

class Input
{
	Global& global;

	std::function<void(size_t)> PassEvent;

	bool mouse_in_window;
	char char_pressed;
	KeyState keys[KEY_ENUM_SIZE];
	double mouse_x;
	double mouse_y;
	int scroll_position;

public:
	Input(Global& global_, const std::function<void(size_t)>& passevent = nullptr);

	void AssignPollFunc(const std::function<void(size_t)>& passevent);

	void Handle_event();

	char charPressed();

	bool wasPressed(Key to_check);
	bool wasReleased(Key to_check);
	bool isUp(Key to_check);
	bool isDown(Key to_check);

	int getScrollWheel();
	int mouseX();
	int mouseY();

	bool mouseInWindow();
};


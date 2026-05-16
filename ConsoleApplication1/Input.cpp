#include "Input.h"
#include <iostream>
#include "Global.h"

Input::Input(Global& global_, const std::function<void(size_t)>& passevent)
    : global(global_)
{
    PassEvent = passevent;

    mouse_in_window = true;

	mouse_x = 0;
	mouse_y = 0;
	char_pressed = 0;

    scroll_position = 0;

	for (int i = 1; i < KEY_ENUM_SIZE; i++)
	{
		keys[i] = UP;
	}
}

void Input::AssignPollFunc(const std::function<void(size_t)>& passevent)
{
    PassEvent = passevent;
}

void Input::Handle_event()
{
    // reset the scroll delta
    scroll_position = 0;

    for (int i = 1; i < KEY_ENUM_SIZE; i++)
    {
        keys[i] = keys[i] == PRESSED ? DOWN : keys[i];
        keys[i] = keys[i] == RELEASED ? UP : keys[i];
    }

    char_pressed = 0;

    auto& window = global.window;

    while (std::optional<sf::Event> event = window.pollEvent())
    {
        // "close requested"
        if (event->is<sf::Event::Closed>())
            window.close();

        // 1. Handle Key Presses
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
        {
            Key changed_id = NO_INPUT;

            switch (keyPressed->scancode)
            {
            case sf::Keyboard::Scancode::Escape:     changed_id = ESC; break;
            case sf::Keyboard::Scancode::Tab:        changed_id = TAB; break;
            case sf::Keyboard::Scancode::CapsLock:   changed_id = CAPSLOCK; break;
            case sf::Keyboard::Scancode::LAlt:       changed_id = ALT; break;
            case sf::Keyboard::Scancode::RAlt:       changed_id = ALT; break;
            case sf::Keyboard::Scancode::LControl:   changed_id = CONTROL; break;
            case sf::Keyboard::Scancode::RControl:   changed_id = CONTROL; break;
            case sf::Keyboard::Scancode::LShift:     changed_id = SHIFT; break;
            case sf::Keyboard::Scancode::RShift:     changed_id = SHIFT; break;
            case sf::Keyboard::Scancode::Enter:      changed_id = ENTER; break;
            case sf::Keyboard::Scancode::PageDown:      changed_id = PAGE_DOWN; break;
            case sf::Keyboard::Scancode::PageUp:      changed_id = PAGE_UP; break;
            case sf::Keyboard::Scancode::Backspace:      changed_id = BACKSPACE; break;
            default: break;
            }

            keys[changed_id] = PRESSED;

            if (PassEvent != nullptr)
                PassEvent(PRESSED +((int)changed_id << 4));
        }

        // 2. Handle Key Releases
        else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
        {
            Key changed_id = NO_INPUT;

            switch (keyReleased->scancode)
            {
            case sf::Keyboard::Scancode::Escape:     changed_id = ESC; break;
            case sf::Keyboard::Scancode::Tab:        changed_id = TAB; break;
            case sf::Keyboard::Scancode::CapsLock:   changed_id = CAPSLOCK; break;
            case sf::Keyboard::Scancode::LAlt:       changed_id = ALT; break;
            case sf::Keyboard::Scancode::RAlt:       changed_id = ALT; break;
            case sf::Keyboard::Scancode::LControl:   changed_id = CONTROL; break;
            case sf::Keyboard::Scancode::RControl:   changed_id = CONTROL; break;
            case sf::Keyboard::Scancode::LShift:     changed_id = SHIFT; break;
            case sf::Keyboard::Scancode::RShift:     changed_id = SHIFT; break;
            case sf::Keyboard::Scancode::Enter:      changed_id = ENTER; break;
            default: break;
            }

            keys[changed_id] = RELEASED;

            if (PassEvent != nullptr)
                PassEvent(RELEASED + ((int)changed_id << 4));
        }
        else if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            char32_t char_32 = textEntered->unicode;

            if (char_32 < 128 and char_32 > 31)
                char_pressed = static_cast<char>(char_32);

            if (PassEvent != nullptr)
                PassEvent(CHARPRESSED);
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Right)
            {
                keys[MOUSE_RIGHT] = PRESSED;
                if (PassEvent != nullptr)
                    PassEvent(PRESSED + (MOUSE_RIGHT << 4));
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            { 
                keys[MOUSE_LEFT] = PRESSED;
                if (PassEvent != nullptr)
                    PassEvent(PRESSED + (MOUSE_LEFT << 4));
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
            { 
                keys[MOUSE_MIDDLE] = PRESSED;
                if (PassEvent != nullptr)
                    PassEvent( PRESSED + (MOUSE_MIDDLE << 4));
            }
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonReleased>())
        {
            if (mouseButtonPressed->button == sf::Mouse::Button::Right)
            { 
                keys[MOUSE_RIGHT] = RELEASED;
                if (PassEvent != nullptr)
                    PassEvent(RELEASED+ (MOUSE_RIGHT << 4));
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Left)
            { 
                keys[MOUSE_LEFT] = RELEASED;
                if (PassEvent != nullptr)
                    PassEvent(RELEASED + ( MOUSE_LEFT << 4));
            }
            if (mouseButtonPressed->button == sf::Mouse::Button::Middle)
            {
                keys[MOUSE_MIDDLE] = RELEASED;
                if (PassEvent != nullptr)
                    PassEvent(RELEASED+ (MOUSE_MIDDLE << 4));
            }
        }
        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>())
        {
            scroll_position = static_cast<int>(mouseWheelScrolled->delta);
            if (PassEvent != nullptr)
                PassEvent(MOUSEWHEEL);
        }
        else if (event->is<sf::Event::MouseEntered>())
            mouse_in_window = true;
            

        else if (event->is<sf::Event::MouseLeft>())
            mouse_in_window = false;

        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
        {
            mouse_x = mouseMoved->position.x;
            mouse_y = mouseMoved->position.y;
        }
        else if (const auto* resized = event->getIf<sf::Event::Resized>())
        {
            // 1. Create a view with the new size
            sf::View view;
            view.setSize({ static_cast<float>(resized->size.x), static_cast<float>(resized->size.y) });

            // 2. Center the view (so 0,0 stays at the top-left)
            view.setCenter({ resized->size.x / 2.f, resized->size.y / 2.f });

            // 3. Apply it to the window (Note the snake_case!)
            window.setView(view);

            global.screenWidth = view.getSize().x;
            global.screenHeight = view.getSize().y;
        }
    }
}

char Input::charPressed()
{
    return char_pressed;
}

bool Input::wasPressed(Key to_check)
{
    return keys[to_check] == PRESSED;
}

bool Input::wasReleased(Key to_check)
{
    return keys[to_check] == RELEASED;
}

bool Input::isUp(Key to_check)
{
    return keys[to_check] == UP;
}

bool Input::isDown(Key to_check)
{
    return keys[to_check] == DOWN;
}

int Input::getScrollWheel()
{
    return scroll_position;
}

int Input::mouseX()
{
    return mouse_x;
}

int Input::mouseY()
{
    return mouse_y;
}

bool Input::mouseInWindow()
{
    return mouse_in_window;
}

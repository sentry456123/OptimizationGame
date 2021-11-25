#include "keyboard.h"

namespace keyboard
{
    std::map<int, bool> pressing, pressed, released;
}

bool keyboard::IsPressing(SDL_Keycode keycode)
{
    return pressing[keycode];
}

bool keyboard::IsPressed(SDL_Keycode keycode)
{
    return pressed[keycode];
}

bool keyboard::IsReleased(SDL_Keycode keycode)
{
    return released[keycode];
}

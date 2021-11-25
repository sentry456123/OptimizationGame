#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include "SDL.h"
#include <map>

namespace keyboard
{
    bool IsPressing(SDL_Keycode keycode);
    bool IsPressed(SDL_Keycode keycode);
    bool IsReleased(SDL_Keycode keycode);

    extern std::map<int, bool> pressing, pressed, released;
}

#endif /* KEYBOARD_H_ */

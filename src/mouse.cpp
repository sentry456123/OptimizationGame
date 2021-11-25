#include "mouse.h"

#include "SDL.h"

namespace mouse
{
	int pressed, released;
}

namespace mouse
{

Vector2i GetPosition(void)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    return Vector2i(x, y);
}

int GetX(void)
{
	int x;
	SDL_GetMouseState(&x, NULL);
	return x;
}

int GetY(void)
{
	int y;
	SDL_GetMouseState(NULL, &y);
	return y;
}

bool IsPressed(int mousecode)
{
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(mousecode);
}

bool IsClicked(int mousecode)
{
	return pressed == mousecode;
}

bool IsReleased(int mousecode)
{
	return released == mousecode;
}

} /* namespace mouse */

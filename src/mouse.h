#ifndef MOUSE_H_
#define MOUSE_H_

#include "SDL.h"
#include "Vector2i.h"

namespace mouse
{
	extern int pressed, released;
    Vector2i GetPosition(void);
	int GetX(void);
	int GetY(void);

	bool IsPressed(int mousecode);
	bool IsClicked(int mousecode);
	bool IsReleased(int mousecode);
}

#endif /* MOUSE_H_ */

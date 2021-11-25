#ifndef GAME_H_
#define GAME_H_

#include "SDL.h"
#include "Player.h"
#include "MapLevel.h"

#define GAME_FPS 60
#define MAPLEVELS_LENGTH 4

namespace game
{
    void Init(const char* title, int width, int height);
    void Quit(void);
    void DoLoop(void);
    void HandleEvents(void);
    void Update(void);
    void Render(void);
    Vector2i GetWindowSize(void);

    extern SDL_Window* window;
    extern SDL_Renderer* renderer;
    extern const char* title;
    extern bool running;

    namespace entities
    {
        Robot& GetCurrnetRobot(void);

        extern Player player;
        extern MapLevel mapLevels[MAPLEVELS_LENGTH];
        extern int currentMapIndex;
    }
}

#endif /* GAME_H_ */

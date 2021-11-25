#include "game.h"
#include "player.h"
#include "keyboard.h"
#include "canvas.h"
#include "SDL_ttf.h"
#include "codeUi.h"
#include <iostream>
#include "mouse.h"
#include "robotController.h"
#include "SDL_image.h"
#include "memoryUi.h"
#include "audio.h"

namespace game
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    const char* title;
    bool running;

    enum class Scene
    {
        OPENING,
        MAIN,
    };

    /**
     * NEVER change scene by modifying this variable!!!
     * Use MoveScene() instead.
     */
    static Scene scene;

    static Scene sceneBuffer;

    static void MoveScene(Scene from, Scene to);

    namespace entities
    {
        static void Init(void);
        static void Quit(void);
        static void Update(void);
        static void Render(void);

        Player player;
        MapLevel mapLevels[MAPLEVELS_LENGTH];
        int currentMapIndex;
    }

    namespace openingScene
    {
        static void Init(void);
        static void Quit(void);
        static void Update(void);
        static void Render(void);

        static void CreateImage(void);
        static void DestroyImage(void);

        static int frameCount;
        static const int FINAL_FRAME = 240;
        static canvas::Image openingImage;
        const char* openingImagePath = "res/opening.png";
    }

    namespace mainScene
    {
        static void Init(void);
        static void Quit(void);
        static void Update(void);
        static void Render(void);
    }
}

namespace game
{

void Init(const char* title, int width, int height)
{
    game::title = title;

    std::cout << "Initializing SDL..." << std::endl;
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO))
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, SDL_GetError(), window
        );
        return;
    }

    std::cout << "Initializing SDL_ttf..." << std::endl;
    if (TTF_Init())
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, TTF_GetError(), window
        );
        return;
    }

    std::cout << "Initializing SDL_image..." << std::endl;
    if (!IMG_Init(IMG_INIT_PNG))
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, IMG_GetError(), window
        );
        return;
    }

    std::cout << "Initializing SDL_mixer..." << std::endl;
    if (!Mix_Init(MIX_INIT_MP3))
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, Mix_GetError(), window
        );
    }

    std::cout << "Creating window..." << std::endl;
    window = SDL_CreateWindow(
        title,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!window)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, SDL_GetError(), window
        );
    }

    std::cout << "Creating renderer..." << std::endl;
    renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
    );
    if (!renderer)
    {
        SDL_ShowSimpleMessageBox(
            SDL_MESSAGEBOX_ERROR,
            title, SDL_GetError(), window
        );
    }

    running = true;

    std::cout << "Initializing entities..." << std::endl;
    entities::Init();
    codeUi::Init();
    canvas::Init(width, height);
    std::cout << "Creating images..." << std::endl;
    openingScene::CreateImage();
    codeUi::CreateImages();
    MapLevel::CreateImages();
    Robot::CreateImage();
    audio::Init();

    scene = Scene::OPENING;
    sceneBuffer = Scene::OPENING;
    openingScene::Init();

    std::cout << "Done!" << std::endl;

}

void Quit(void)
{
    entities::Quit();
    codeUi::DestroyImages();
    MapLevel::DestroyImages();
    Robot::DestroyImage();
    openingScene::DestroyImage();
    audio::Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

void DoLoop(void)
{
    Uint32 stop, start;
    start = SDL_GetTicks();
    while (running)
    {
        HandleEvents();
        Update();
        Render();
        stop = SDL_GetTicks();
        if (stop - start < 1000 / GAME_FPS)
            SDL_Delay(1000 / GAME_FPS - (stop - start));
        start = SDL_GetTicks();
    }
}

static void ToggleFullscreen(void)
{
    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    bool isFullscreen = SDL_GetWindowFlags(game::window) & fullscreenFlag;
    SDL_SetWindowFullscreen(game::window, isFullscreen ? 0 : fullscreenFlag);
}

void HandleEvents(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    keyboard::pressed = {};
    keyboard::released = {};
    mouse::pressed = {};
    mouse::released = {};
    switch (event.type)
    {
    case SDL_KEYDOWN:
        keyboard::pressing[event.key.keysym.sym] = true;
        keyboard::pressed[event.key.keysym.sym] = true;
		switch (event.key.keysym.sym)
		{
        case SDLK_F11:
            ToggleFullscreen();
            break;
		case SDLK_ESCAPE:
			running = false;
			break;
		}
		break;
    case SDL_KEYUP:
        keyboard::pressing[event.key.keysym.sym] = false;
        keyboard::released[event.key.keysym.sym] = true;
        break;
    case SDL_MOUSEBUTTONDOWN:
	    mouse::pressed = event.button.button;
		break;
	case SDL_MOUSEBUTTONUP:
		mouse::released = event.button.button;
		break;
    case SDL_QUIT:
        running = false;
        break;
    default:
        break;
    }
}

void Update(void)
{
    canvas::Update();
    scene = sceneBuffer;
    switch (scene)
    {
    case Scene::OPENING:
        openingScene::Update();
        break;
    case Scene::MAIN:
        mainScene::Update();
        break;
    }
}

void Render(void)
{
    canvas::SetColor(COLOR_BLACK);
    SDL_RenderClear(renderer);

    canvas::SetColor(Color(10, 10, 10));
    canvas::FillRect(0, 0, canvas::width, canvas::height);

    switch (scene)
    {
    case Scene::OPENING:
        openingScene::Render();
        break;
    case Scene::MAIN:
        mainScene::Render();
        break;
    }

    SDL_RenderPresent(renderer);
}

static void MoveScene(Scene from, Scene to)
{
    sceneBuffer = to;

    switch (from)
    {
    case Scene::OPENING: openingScene::Quit(); break;
    case Scene::MAIN:    mainScene::Quit();    break;
    }

    switch (to)
    {
    case Scene::OPENING: openingScene::Init(); break;
    case Scene::MAIN:    mainScene::Init();    break;
    }
}

Vector2i GetWindowSize(void)
{
    int x, y;
    SDL_GetWindowSize(window, &x, &y);
    return Vector2i(x, y);
}

static void entities::Init(void)
{
    currentMapIndex = 0;
    player.Init();
    for (int i = 0; i < MAPLEVELS_LENGTH; i++)
        mapLevels[i].Init(i);
}

static void entities::Quit(void)
{
    for (int i = 0; i < MAPLEVELS_LENGTH; i++)
        mapLevels[i].Quit();
}

static void entities::Update(void)
{
    for (int i = 0; i < MAPLEVELS_LENGTH; i++)
        mapLevels[i].Update();
}

static void entities::Render(void)
{
    mapLevels[currentMapIndex].Render();
}

Robot& entities::GetCurrnetRobot(void)
{
    return mapLevels[currentMapIndex].robot;
}

static void openingScene::Init(void)
{

}

static void openingScene::Quit(void)
{

}

static void openingScene::Update(void)
{
    frameCount++;
    if (frameCount > FINAL_FRAME)
        MoveScene(Scene::OPENING, Scene::MAIN);
}

static void openingScene::Render(void)
{
    Uint8 colorBuffer;
    if (frameCount < FINAL_FRAME / 4)
        colorBuffer = frameCount * 4;
    else if (frameCount < FINAL_FRAME * 3 / 4)
        colorBuffer = FINAL_FRAME;
    else if (frameCount < FINAL_FRAME)
        colorBuffer = frameCount * (-4) + FINAL_FRAME * 4;
    else
        colorBuffer = 0;
    SDL_SetTextureColorMod(openingImage.GetTexture(), colorBuffer, colorBuffer, colorBuffer);
    openingImage.Render(0, 0, canvas::width, canvas::height);
}

static void openingScene::CreateImage(void)
{
    openingImage.Create(openingImagePath);
}

static void openingScene::DestroyImage(void)
{
    openingImage.Destroy();
}

static void mainScene::Init(void)
{
    audio::BGM::Loop();
}

static void mainScene::Quit(void)
{
    audio::BGM::Stop();
}

static void mainScene::Update(void)
{
    entities::Update();
    mapSwitcher::Update();
    codeUi::Update();
    robotController::Update();
    memoryUi::Update();
}

static void mainScene::Render(void)
{
    entities::Render();
    mapSwitcher::Render();
    codeUi::Render();
    robotController::Render();
    memoryUi::Render();
}

} /* namespace game */

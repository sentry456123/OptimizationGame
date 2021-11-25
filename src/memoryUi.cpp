#include "memoryUi.h"
#include "shape.h"
#include "game.h"
#include "MapLevel.h"
#include "canvas.h"
#include <sstream>

static int GetTitleSize(void)
{
    return canvas::height / 20;
}

static int GetTextSize(void)
{
    return canvas::height / 30;
}

namespace memoryUi
{
    static Robot& GetRobot(void);

    shape::Rectangle transform;
}

void memoryUi::Update(void)
{
    int mapSize = MAP_SIZE * MapLevel::GetCellSize();
    Vector2i position = Vector2i(0, mapSize);
    Vector2i size = Vector2i(mapSize, canvas::height - mapSize);
    transform = shape::Rectangle(position, size);
}

void memoryUi::Render(void)
{
    using namespace canvas;
    SetColor(COLOR_WHITE);
    DrawText(
        "MEMORY",
        FONT_MAIN_PATH,
        transform.position,
        GetTitleSize(),
        TextAlign::TOP_LEFT
    );
    SetColor(COLOR_GRAY);
    FillRect(transform.position + Vector2i(0, GetTitleSize()), Vector2i(MAX_MEMORY_LENGTH * GetTextSize(), GetTextSize()));
    SetColor(COLOR_RED);
    FillRect(
        transform.position + Vector2i(GetTextSize() * GetRobot().memory.ptr, GetTitleSize() + GetTextSize()),
        Vector2i(GetTextSize())
    );
    SetColor(COLOR_WHITE);
    for (int i = 0; i < MAX_MEMORY_LENGTH; i++)
    {
        DrawRect(transform.position + Vector2i(GetTextSize() * i, GetTitleSize()), Vector2i(GetTextSize()));
        DrawRect(transform.position + Vector2i(GetTextSize() * i, GetTitleSize() + GetTextSize()), Vector2i(GetTextSize()));
        DrawText(
            std::to_string(i).c_str(),
            FONT_MAIN_PATH,
            transform.position + Vector2i(GetTextSize() * i, GetTitleSize()),
            GetTextSize(),
            TextAlign::TOP_LEFT
        );
        DrawText(
            std::to_string(GetRobot().memory.elements[i]).c_str(),
            FONT_MAIN_PATH,
            transform.position + Vector2i(GetTextSize() * i, GetTitleSize() + GetTextSize()),
            GetTextSize(),
            TextAlign::TOP_LEFT
        );
    }
}

static Robot& memoryUi::GetRobot(void)
{
    return game::entities::GetCurrnetRobot();
}


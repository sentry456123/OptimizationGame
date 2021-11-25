#include "MapLevel.h"
#include "canvas.h"
#include <iostream>
#include "Button.h"
#include "game.h"

#define IMAGE_CRATE_PATH "res/crate.png"
#define IMAGE_CRATE_SIZE 100

#define IMAGE_TERRAIN_PATH "res/terrain.png"
#define IMAGE_TERRAIN_SIZE 100

namespace terrain
{
    const int block = 0x00000000;
    const int empty = 0x00000001;
    const int hole  = 0x00000002;

    const int isWalkable = empty | hole;
}

namespace mapSwitcher
{
    static Button left, right;
}

canvas::Image MapLevel::terrainImage;
canvas::Image MapLevel::crateImage;

void MapLevel::InitCrates(int cratesLength)
{
    this->cratesLength = cratesLength;
    crates = new int[cratesLength];
    initialCrates = new int[cratesLength];
}

void MapLevel::Init(int index)
{
    m_index = index;

    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++)
        terrain[i] = terrain::empty;
    for (int i = 0; i < MAP_SIZE; i++)
    {
        terrain[i] = terrain::block;
        terrain[i + MAP_SIZE * (MAP_SIZE - 1)] = terrain::block;
        terrain[i * MAP_SIZE] = terrain::block;
        terrain[(i + 1) * MAP_SIZE - 1] = terrain::block;
    }

    switch (index)
    {
    case 0:
        terrain[Vector2i(4, 1).ToCoord()] = terrain::hole;
        initialRobotCoord = Vector2i(4, 7).ToCoord();
        initialRobotDirection = MAP_DIRECTION_UP;
        InitCrates(1);
        initialCrates[0] = Vector2i(4, 4).ToCoord();
        break;
    case 1:
        terrain[Vector2i(7, 2).ToCoord()] = terrain::hole;
        initialRobotCoord = Vector2i(1, 7).ToCoord();
        initialRobotDirection = MAP_DIRECTION_RIGHT;
        InitCrates(1);
        initialCrates[0] = Vector2i(7, 3).ToCoord();
        break;
    case 2:
        terrain[Vector2i(3, 4).ToCoord()] = terrain::hole;
        initialRobotCoord = Vector2i(3, 6).ToCoord();
        initialRobotDirection = MAP_DIRECTION_UP;
        InitCrates(1);
        initialCrates[0] = Vector2i(4, 5).ToCoord();
        break;
    case 3:
        terrain[Vector2i(1, 1).ToCoord()] = terrain::hole;
        terrain[Vector2i(2, 1).ToCoord()] = terrain::hole;
        initialRobotCoord = Vector2i(3, 3).ToCoord();
        initialRobotDirection = MAP_DIRECTION_LEFT;
        InitCrates(2);
        initialCrates[0] = Vector2i(1, 2).ToCoord();
        initialCrates[1] = Vector2i(2, 2).ToCoord();
        break;
    }

    robot.Init(this);
    robot.coord = initialRobotCoord;
    robot.direction = initialRobotDirection;
    Reset();
}

void MapLevel::Quit(void)
{
    delete[] crates;
    delete[] initialCrates;
}

void MapLevel::Update(void)
{
    robot.Update();
}

void MapLevel::Render(void) const
{
    for (int i = 0; i < MAP_SIZE * MAP_SIZE; i++)
    {
        Vector2i position = CoordToVector2i(i) * GetCellSize();

        shape::Rectangle terrainImageFrame = shape::Rectangle(Vector2i(0), Vector2i(IMAGE_TERRAIN_SIZE));
        shape::Rectangle terrainImageTransform = shape::Rectangle(position, Vector2i(GetCellSize()));

        switch (terrain[i])
        {
        case terrain::empty:
            terrainImage.Render(terrainImageFrame, terrainImageTransform);
            break;
        case terrain::block:
            terrainImage.Render(terrainImageFrame.AddedPosition(Vector2i(IMAGE_TERRAIN_SIZE, 0)), terrainImageTransform);
            break;
        case terrain::hole:
            terrainImage.Render(terrainImageFrame.AddedPosition(Vector2i(IMAGE_TERRAIN_SIZE * 2, 0)), terrainImageTransform);
            break;
        }
    }
    for (int i = 0; i < cratesLength; i++)
    {
        if (!crates[i]) continue;
        Vector2i position = CoordToVector2i(crates[i]) * GetCellSize();
        crateImage.Render(position, Vector2i(GetCellSize()));
    }

    {
        std::string buffer;
        buffer += "Level: ";
        buffer += std::to_string(m_index + 1);
        canvas::SetColor(COLOR_WHITE);
        canvas::DrawText(
            buffer.c_str(),
            FONT_MAIN_PATH,
            Vector2i(0),
            GetCellSize(),
            canvas::TextAlign::TOP_LEFT
        );
    }

    if (robot.coord)
        robot.Render();
}

void MapLevel::Reset(void)
{
    for (int i = 0; i < cratesLength; i++)
        crates[i] = initialCrates[i];
}

int MapLevel::GetCratesLength(void)
{
    return cratesLength;
}

int MapLevel::GetCellSize(void)
{
    return canvas::height / 15;
}

void MapLevel::CreateImages(void)
{
    terrainImage.Create(IMAGE_TERRAIN_PATH);
    crateImage.Create(IMAGE_CRATE_PATH);
}

void MapLevel::DestroyImages(void)
{
    terrainImage.Destroy();
    crateImage.Destroy();
}

void mapSwitcher::Update(void)
{
    int cell = MapLevel::GetCellSize();
    left.Init(
        Vector2i(0, cell * 4),
        Vector2i(cell),
        "<",
        BUTTON_DEFAULT
    );
    right.Init(
        Vector2i(cell * 8, cell * 4),
        Vector2i(cell),
        ">",
        BUTTON_DEFAULT
    );
    left.Update();
    right.Update();
    if (left.IsPushed())
    {
        if (game::entities::currentMapIndex > 0)
            game::entities::currentMapIndex--;
    }
    if (right.IsPushed())
    {
        if (game::entities::currentMapIndex < MAPLEVELS_LENGTH - 1)
            game::entities::currentMapIndex++;
    }
}

void mapSwitcher::Render(void)
{
    left.Render();
    right.Render();
}

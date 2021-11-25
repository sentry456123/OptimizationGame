#include "robotController.h"
#include "Button.h"
#include "canvas.h"
#include "MapLevel.h"
#include "game.h"

#define BUTTON_LENGTH 2

namespace robotController
{
    static int GetButtonHeight(void);
    static const char* IndexToChar(int index);

    static shape::Rectangle transform;
    static Button buttons[BUTTON_LENGTH];

    namespace index
    {
        static const int START_OR_STOP_ROBOT = 0;
        static const int LOOP_ON_OR_OFF = 1;
    }
}

int robotController::GetButtonHeight(void)
{
    return canvas::height / 15;
}

const char* robotController::IndexToChar(int index)
{
    Robot& robot = game::entities::GetCurrnetRobot();
    switch (index)
    {
    case index::START_OR_STOP_ROBOT:
        if (robot.IsRunning())
            return "Stop Robot";
        else return "Start Robot";
    case index::LOOP_ON_OR_OFF:
        if (robot.loop)
            return "Loop On";
        else return "Loop Off";
    }
    return " ";
}

void robotController::Init(void)
{

}

void robotController::Update(void)
{
    transform.position = Vector2i((canvas::width + MapLevel::GetCellSize() * MAP_SIZE) / 2, MapLevel::GetCellSize() * MAP_SIZE);
    transform.size = Vector2i((canvas::width - MapLevel::GetCellSize() * MAP_SIZE) / 2, canvas::height - MapLevel::GetCellSize() * MAP_SIZE);

    for (int i = 0; i < BUTTON_LENGTH; i++)
    {
        buttons[i].Init(
            transform.position + Vector2i(0, i * GetButtonHeight()),
            Vector2i(transform.size.x, GetButtonHeight()),
            IndexToChar(i),
            BUTTON_BORDER
        );
        buttons[i].Update();
        if (buttons[i].IsPushed())
        {
            Robot& robot = game::entities::GetCurrnetRobot();

            switch (i)
            {
            case index::START_OR_STOP_ROBOT:
                if (robot.IsRunning()) robot.Exit();
                else robot.Reset();
                break;
            case index::LOOP_ON_OR_OFF:
                robot.loop = !robot.loop;
                break;
            }
        }
    }
}

void robotController::Render(void)
{
    for (int i = 0; i < BUTTON_LENGTH; i++)
    {
        buttons[i].Render();
    }
}

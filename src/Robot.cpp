#include "Robot.h"
#include "MapLevel.h"
#include "canvas.h"
#include "game.h"
#include <iostream>

#define IMAGE_ROBOT_PATH "res/robot.png"
#define IMAGE_ROBOT_SIZE 100

canvas::Image Robot::robotImage;

void Robot::Init(MapLevel* mapLevel)
{
    this->mapLevel = mapLevel;
    delay = 15;
    m_codeLength = 0;

    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        code.actions[i] = NULL;
        code.args[i] = 0;
    }

    Reset();
    m_running = false;
}

void Robot::Update(void)
{
    if (!m_running)
        return;
    if (m_interval >= delay)
    {
        Execute();
        m_interval = 0;
    }
    else m_interval++;
}

void Robot::Render(void) const
{
    const int cell = MapLevel::GetCellSize();
    Vector2i pos = CoordToVector2i(coord) * cell;

    shape::Rectangle imageFrame = shape::Rectangle(Vector2i(0), Vector2i(IMAGE_ROBOT_SIZE));
    shape::Rectangle imageTransform = shape::Rectangle(pos, Vector2i(cell));

    switch (direction)
    {
    case MAP_DIRECTION_UP:    robotImage.Render(imageFrame,                                                  imageTransform); break;
    case MAP_DIRECTION_RIGHT: robotImage.Render(imageFrame.AddedPosition(Vector2i(IMAGE_ROBOT_SIZE, 0)),     imageTransform); break;
    case MAP_DIRECTION_DOWN:  robotImage.Render(imageFrame.AddedPosition(Vector2i(IMAGE_ROBOT_SIZE * 2, 0)), imageTransform); break;
    case MAP_DIRECTION_LEFT:  robotImage.Render(imageFrame.AddedPosition(Vector2i(IMAGE_ROBOT_SIZE * 3, 0)), imageTransform); break;
    }
}

int Robot::GetReadingLine(void) const
{
    return m_readingLine;
}

void Robot::SetReadingLine(const int line)
{
    m_readingLine = line;
}

void Robot::Reset(void)
{
    m_running = true;
    m_readingLine = 0;
    coord = mapLevel->initialRobotCoord;
    direction = mapLevel->initialRobotDirection;
    for (int i = 0; i < MAX_MEMORY_LENGTH; i++)
        memory.elements[i] = 0;
    memory.ptr = 0;

    mapLevel->Reset();
}

void Robot::Exit(void)
{
    m_running = false;
}

bool Robot::IsRunning(void)
{
    return m_running;
}

void Robot::SwapAction(const int index1, const int index2)
{
    {
        action::Function temp = code.actions[index1];
        code.actions[index1] = code.actions[index2];
        code.actions[index2] = temp;
    }
    {
        int temp = code.args[index1];
        code.args[index1] = code.args[index2];
        code.args[index2] = temp;
    }
}

/**
 *  Returns true if successful.
 */
bool Robot::AddAction(const action::Function action)
{
    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        if (code.actions[i]) continue;
        DeleteAction(i);
        code.actions[i] = action;
        if (action == action::GoTo)
        {
            code.args[i] = 0;
        }
        return true;
    }
    return false;
}

/**
 *  Returns true if successful.
 */
bool Robot::PackAction(const int index)
{
    if (code.actions[index])
        return false;
    for (int i = index; i < MAX_CODE_LENGTH - 1; i++)
        SwapAction(i, i + 1);
    return true;
}

void Robot::DeleteAction(const int index)
{
    code.actions[index] = NULL;
    code.args[index] = 0;
}

void Robot::StoreBackup(void)
{
    codeBackups[codeBackupPtr] = code;

    if (codeBackupPtr >= CODE_BACKUP_LENGTH - 1) codeBackupPtr = 0;
    else codeBackupPtr++;
}

void Robot::RestoreBackup(void)
{
    if (codeBackupPtr < 0) codeBackupPtr = CODE_BACKUP_LENGTH - 1;
    else codeBackupPtr--;

    code = codeBackups[codeBackupPtr];
}

void Robot::Execute(void)
{
    if (ShouldExit())
    {
        bool isSuccess = true;
        for (int i = 0; i < mapLevel->cratesLength; i++)
        {
            if (mapLevel->crates)
            {
                isSuccess = false;
                break;
            }
        }
        if (!coord) isSuccess = false;
        if (isSuccess)
        {
            std::cout << "Success!" << std::endl;
        }
        loop ? Reset() : Exit();
        return;
    }
    if (code.actions[m_readingLine]) code.actions[m_readingLine](*this);
    m_readingLine++;
}

bool Robot::ShouldExit(void)
{
    if (m_readingLine >= MAX_CODE_LENGTH)
        return true;
    if (code.actions[m_readingLine] == action::Exit)
        return true;
    if (!coord)
        return true;
    return false;
}

void Robot::CreateImage(void)
{
    robotImage.Create(IMAGE_ROBOT_PATH);
}

void Robot::DestroyImage(void)
{
    robotImage.Destroy();
}

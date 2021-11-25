#include "action.h"
#include "game.h"
#include "MapLevel.h"
#include <typeinfo>
#include <iostream>
#include <string>

static int GetArg(const Robot& robot)
{
    return robot.code.args[robot.GetReadingLine()];
}

static int GetMemory(const Robot& robot)
{
    return robot.memory.elements[robot.memory.ptr];
}

static void SetMemory(Robot& robot, int value)
{
    robot.memory.elements[robot.memory.ptr] = value;
}

static void ExecuteTill0(Robot& robot, void(*act)(Robot&))
{
    int* memory = &robot.memory.elements[robot.memory.ptr];
    if (*memory <= 0) return;
    act(robot);
    (*memory)--;
    robot.SetReadingLine(robot.GetReadingLine() - 1);
}

static void MoveForwardOnce(Robot& robot)
{
    MapLevel* mapLevel = robot.mapLevel;
    const int destination = mapLevel->terrain[robot.coord + robot.direction];

    if (!(destination & terrain::isWalkable)) return;
    bool isCrateThere = false;
    int* targetCrate;
    for (int i = 0; i < mapLevel->cratesLength; i++)
    {
        if (mapLevel->crates[i] == robot.coord + robot.direction)
        {
            isCrateThere = true;
            targetCrate = &mapLevel->crates[i];
            break;
        }
    }
    if (isCrateThere)
    {
        const int overCrateTerrain = mapLevel->terrain[robot.coord + robot.direction * 2];
        if (!(overCrateTerrain & terrain::isWalkable)) return;
        for (int i = 0; i < mapLevel->cratesLength; i++)
            if (mapLevel->crates[i] == robot.coord + robot.direction * 2) return;
        *targetCrate += robot.direction;
        if (mapLevel->terrain[*targetCrate] & terrain::hole)
            *targetCrate = 0;
    }
    robot.coord += robot.direction;
    if (mapLevel->terrain[robot.coord] & terrain::hole)
        robot.coord = 0;
}

void action::MoveForward(Robot& robot)
{
    ExecuteTill0(robot, MoveForwardOnce);
}

/**
 *  Direction must be 1 on right,
 *  -1 on left.
 */
static void TurnRightOnce(Robot& robot)
{
    switch (robot.direction)
    {
    case MAP_DIRECTION_LEFT:  robot.direction = MAP_DIRECTION_UP;    break;
    case MAP_DIRECTION_UP:    robot.direction = MAP_DIRECTION_RIGHT; break;
    case MAP_DIRECTION_RIGHT: robot.direction = MAP_DIRECTION_DOWN;  break;
    case MAP_DIRECTION_DOWN:  robot.direction = MAP_DIRECTION_LEFT;  break;
    }
}

void action::TurnRight(Robot& robot)
{
    ExecuteTill0(robot, TurnRightOnce);
}

void action::GoTo(Robot& robot)
{
    int arg = GetArg(robot);
    robot.SetReadingLine(arg - 1);
}

void action::GoToValue(Robot& robot)
{
    int memory = GetMemory(robot);
    robot.SetReadingLine(memory - 1);
}

void action::ChangeValueTo(Robot& robot)
{
    int arg = GetArg(robot);
    SetMemory(robot, arg);
}

void action::ChangeValueBy(Robot& robot)
{
    int arg = GetArg(robot);
    SetMemory(robot, GetMemory(robot) + arg);
}

void action::MovePointerTo(Robot& robot)
{
    int arg = GetArg(robot);
    if (arg < 0)
    {
        robot.memory.ptr = 0;
        return;
    }
    if (arg >= MAX_MEMORY_LENGTH)
    {
        robot.memory.ptr = MAX_MEMORY_LENGTH - 1;
        return;
    }
    robot.memory.ptr = arg;
}

void action::MovePointerBy(Robot& robot)
{
    int arg = GetArg(robot);
    if (robot.memory.ptr + arg < 0)
    {
        robot.memory.ptr = 0;
        return;
    }
    if (robot.memory.ptr + arg >= MAX_MEMORY_LENGTH)
    {
        robot.memory.ptr = MAX_MEMORY_LENGTH - 1;
        return;
    }
    robot.memory.ptr += arg;
}

void action::CopyValueFrom(Robot& robot)
{
    int arg = GetArg(robot);
    int memory = robot.memory.elements[robot.memory.ptr + arg];
    SetMemory(robot, memory);
}

void action::IfThen(Robot& robot)
{
    int arg = GetArg(robot);
    int memory = GetMemory(robot);
    if (arg != memory)
        robot.SetReadingLine(robot.GetReadingLine() + 1);
}

void action::IfNotThen(Robot& robot)
{
    int arg = GetArg(robot);
    int memory = GetMemory(robot);
    if (arg == memory)
        robot.SetReadingLine(robot.GetReadingLine() + 1);
}

void action::Exit(Robot&) {}

const char* action::ToConstCharPtr(Function action, int arg)
{
    if (action == action::Exit)        return "Exit";
    if (action == action::MoveForward) return "Move Forward";
    if (action == action::TurnRight)   return "Turn Right";
    if (action == action::GoTo)
    {
        if (!arg) return "Go To 0";
        const char* text = "Go To ";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::GoToValue)   return "Go To Value";
    if (action == action::ChangeValueTo)
    {
        if (!arg) return "Change Value To 0";
        const char* text = "Change Value To ";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::ChangeValueBy)
    {
        if (!arg) return "Change Value By 0";
        const char* text = "Change Value By ";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::MovePointerTo)
    {
        if (!arg) return "Move Pointer To 0";
        const char* text = "Move Pointer To ";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::MovePointerBy)
    {
        if (!arg) return "Move Pointer By 0";
        const char* text = "Move Pointer By ";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::CopyValueFrom)
    {
        if (!arg) return "Copy Value From ptr+0";
        std::string text;
        if (arg < 0) text = "Copy Value From ptr";
        else text = "Copy Value From ptr+";
        std::string buffer = text + std::to_string(arg);
        return buffer.c_str();
    }
    if (action == action::IfThen)
    {
        if (!arg) return "If 0 Then";
        const char* text1 = "If ";
        const char* text2 = " Then";
        std::string buffer = text1 + std::to_string(arg) + text2;
        return buffer.c_str();
    }
    if (action == action::IfNotThen)
    {
        if (!arg) return "If Not 0 Then";
        const char* text1 = "If Not ";
        const char* text2 = " Then";
        std::string buffer = text1 + std::to_string(arg) + text2;
        return buffer.c_str();
    }
    return " ";
}

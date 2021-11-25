#include "game.h"
#include <typeinfo>
#include <iostream>

int main(int argc, char** argv)
{
    game::Init("Optimization", 1280, 720);
    game::DoLoop();
    game::Quit();
    return 0;
}

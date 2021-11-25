#ifndef MAPLEVEL_H_
#define MAPLEVEL_H_

#include "Vector2i.h"
#include "Robot.h"
#include "canvas.h"

#define MAP_SIZE 9

#define MAP_DIRECTION_UP -MAP_SIZE
#define MAP_DIRECTION_DOWN MAP_SIZE
#define MAP_DIRECTION_LEFT -1
#define MAP_DIRECTION_RIGHT 1

namespace terrain
{
    extern const int block;
    extern const int empty;
    extern const int hole;

    extern const int isWalkable;
}

namespace mapSwitcher
{
    void Update();
    void Render();
}

class MapLevel
{
public:
    void Init(int index);
    void Quit(void);
    void Update(void);
    void Render(void) const;
    void Reset(void);
    int GetCratesLength(void);

    int terrain[MAP_SIZE * MAP_SIZE];
    Robot robot;
    int* crates;
    int cratesLength;
    int initialRobotCoord;
    int initialRobotDirection;
    int* initialCrates;

    static int GetCellSize(void);
    static void CreateImages(void);
    static void DestroyImages(void);

    static canvas::Image terrainImage, crateImage;
protected:
    void InitCrates(int cratesLength);

    int m_index;
};

#endif /* MAPLEVEL_H_ */

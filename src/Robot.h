#ifndef ROBOT_H_
#define ROBOT_H_

#include "action.h"
#include "canvas.h"

#define MAX_CODE_LENGTH 50
#define MAX_MEMORY_LENGTH 10
#define CODE_BACKUP_LENGTH 10

class MapLevel;

class Robot
{
public:
    void Init(MapLevel* mapLevel);
    void Update(void);
    void Render(void) const;
    void Reset(void);
    void Exit(void);
    bool IsRunning(void);

    void SwapAction(const int index1, const int index2);
    bool AddAction(const action::Function);
    bool PackAction(const int index);
    void DeleteAction(const int index);
    int GetReadingLine(void) const;
    void SetReadingLine(const int);
    void StoreBackup(void);
    void RestoreBackup(void);

    MapLevel* mapLevel;
    int coord;
    int direction;
    bool loop;
    int delay;
    struct Code
    {
        action::Function actions[MAX_CODE_LENGTH];
        int args[MAX_CODE_LENGTH];
    } code;
    Code codeBackups[CODE_BACKUP_LENGTH];
    int codeBackupPtr;
    struct Memory
    {
        int elements[MAX_MEMORY_LENGTH];
        int ptr;
    } memory;

    static void CreateImage(void);
    static void DestroyImage(void);

    static canvas::Image robotImage;
protected:
    void Execute(void);
    bool ShouldExit(void);

    int m_readingLine;
    int m_interval;
    int m_codeLength;
    bool m_running;

};

#endif /* ROBOT_H_ */

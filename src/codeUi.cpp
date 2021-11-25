#include "codeUi.h"
#include "MapLevel.h"
#include "game.h"
#include "canvas.h"
#include <iostream>
#include "Robot.h"
#include "shape.h"
#include "mouse.h"
#include <sstream>

#define IMAGE_SIMPLE_ACTION_EDIT_PATH "res/simple_action_edit.png"
#define IMAGE_SIMPLE_ACTION_EDIT_SIZE 100

static int GetActionHeight()
{
    return canvas::height / 30;
}

/**
 *  Code Ui is where you can watch the current robot's actions.
 */
namespace codeUi
{
    static Robot& GetRobot(void);

    static shape::Rectangle transform;
    static int startPoint;

    namespace scrollBar
    {
        static void Update(void);
        static void Render(void);
        static int GetBarWidth(void);

        static shape::Rectangle transform;
        static Button up, down;
    }

    /**
     *  You can add as many actions as capable by this module.
     */
    namespace addAction
    {
        static void Update(void);
        static void Render(void);
        static action::Function IndexToAction(int index);
        static int GetTitleHeight(void);

        static shape::Rectangle transform;
        static const int buttonLength = 12;
        static Button buttons[buttonLength];
        static const char* title = "Add Action";
    }

    /**
     *  A Simple Edit Action module can allow you to move actions upside down and delete them.
     */
    namespace simpleEditAction
    {
        static void Update(void);
        static void Render(void);

        static void CreateImages(void);
        static void DestroyImages(void);

        struct Buttons
        {
            void Update(const shape::Rectangle& transform, int index);
            void Render(void) const;
            void UpAction(int index), DownAction(int index), DestroyAction(int index);

            shape::Rectangle transform;
            ImageButton up, down, destroy;
            ImageButton* buttonPtrs[3] = {&up, &down, &destroy}; // The order is very important
            static const int BUTTON_LENGTH = sizeof(buttonPtrs) / sizeof(*buttonPtrs);
        };

        Buttons buttonsArray[MAX_CODE_LENGTH];
        canvas::Image buttonImage;
    }

    /**
     *  An Action Editor allow you to manage your code more efficiently,
     *  for instance, packing actions.
     */
    namespace actionEditor
    {
        static void Init(void);
        static void Update(void);
        static void Render(void);
        static int GetTitleHeight(void);

        shape::Rectangle transform;
        static const char* title = "Edit Action";
        static int selectedAction;
        static Button packAction, deleteAndPackAction;
        static Button clearAll, undo;
        static Button incrementArg, decrementArg;

        static const int BUTTON_LENGTH = 4;
        static Button* buttonPtrs[BUTTON_LENGTH] = {&packAction, &deleteAndPackAction, &clearAll, &undo}; // The order doesn't matter.
        static const int WITH_ARG_FUNCS_LENGTH = 8;
        static action::Function withArgFuncs[WITH_ARG_FUNCS_LENGTH] = {
            action::GoTo,
            action::ChangeValueTo, action::ChangeValueBy,
            action::MovePointerTo, action::MovePointerBy,
            action::CopyValueFrom,
            action::IfThen, action::IfNotThen
        };
        static bool isWithArg;
    }
}

namespace codeUi
{

void Init(void)
{
    actionEditor::Init();
}

void Update(void)
{
    transform.position = Vector2i(MapLevel::GetCellSize() * MAP_SIZE, 0);
    transform.size = Vector2i((canvas::width - MapLevel::GetCellSize() * MAP_SIZE) / 2, MapLevel::GetCellSize() * MAP_SIZE);

    scrollBar::Update();
    simpleEditAction::Update();
    addAction::Update();
    actionEditor::Update();
}

void Render(void)
{
    using namespace canvas;
    Vector2i position = transform.position;
    Vector2i size = transform.size;

    SetColor(COLOR_DARK_GRAY);
    FillRect(position, size);

    Robot& robot = GetRobot();

    if ((robot.GetReadingLine() - startPoint) * GetActionHeight() < transform.position.y + transform.size.y && (robot.GetReadingLine() - startPoint) * GetActionHeight() >= transform.position.y)
    {
        SetColor(COLOR_RED);
        FillRect(position + Vector2i(0, (robot.GetReadingLine() - startPoint) * GetActionHeight()), Vector2i(size.x, GetActionHeight()) - Vector2i(scrollBar::GetBarWidth(), 0));
    }

    SetColor(COLOR_WHITE);
    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        if ((i + 1) * GetActionHeight() > transform.size.y)
            break;
        std::ostringstream buffer;
        buffer << (i + startPoint) << " " << action::ToConstCharPtr(robot.code.actions[i + startPoint], robot.code.args[i + startPoint]);
        DrawText(
            buffer.str().c_str(),
            FONT_MAIN_PATH,
            position + Vector2i(0, i * GetActionHeight()),
            GetActionHeight(),
            TextAlign::TOP_LEFT
        );
    }

    scrollBar::Render();
    simpleEditAction::Render();
    addAction::Render();
    actionEditor::Render();
}

void CreateImages(void)
{
    simpleEditAction::CreateImages();
}

void DestroyImages(void)
{
    simpleEditAction::DestroyImages();
}

static Robot& GetRobot(void)
{
    return game::entities::GetCurrnetRobot();
}

static void scrollBar::Update(void)
{
    static int oldMapIndex;
    int newMapIndex = game::entities::currentMapIndex;
    if (oldMapIndex != newMapIndex)
        startPoint = 0;
    oldMapIndex = newMapIndex;

    transform.position = codeUi::transform.position + Vector2i(codeUi::transform.size.x - GetBarWidth(), 0);
    transform.size = Vector2i(GetBarWidth(), codeUi::transform.size.y);

    up.Init(
        transform.position,
        Vector2i(transform.size.x, GetBarWidth()),
        "U",
        BUTTON_BORDER
    );
    down.Init(
        transform.position + Vector2i(0, transform.size.y - GetBarWidth()),
        Vector2i(transform.size.x, GetBarWidth()),
        "D",
        BUTTON_BORDER
    );
    up.Update();
    down.Update();
    if (up.IsPushed())
    {
        if (startPoint > 0)
            startPoint--;
    }
    if (down.IsPushed())
    {
        if (startPoint < MAX_CODE_LENGTH - 1 - codeUi::transform.size.x / GetActionHeight())
            startPoint++;
    }
}

static void scrollBar::Render(void)
{
    canvas::SetColor(COLOR_WHITE);
    canvas::DrawRect(transform);

    up.Render();
    down.Render();
}

static int scrollBar::GetBarWidth(void)
{
    return canvas::width / 60;
}

static void addAction::Update(void)
{
    transform.position = codeUi::transform.position + Vector2i(0, codeUi::transform.size.y);
    transform.size = Vector2i(codeUi::transform.size.x, canvas::height - MAP_SIZE * MapLevel::GetCellSize());

    for (int i = 0; i < buttonLength; i++)
    {
        int x = (i % 2) * codeUi::transform.size.x / 2;
        int y = MapLevel::GetCellSize() * MAP_SIZE + (i / 2) * GetActionHeight();
        buttons[i].Init(
            codeUi::transform.position + Vector2i(x, y) + Vector2i(0, GetTitleHeight()),
            Vector2i(codeUi::transform.size.x / 2, GetActionHeight()),
            action::ToConstCharPtr(IndexToAction(i), 0),
            BUTTON_BORDER
        );
        buttons[i].Update();
    }
    for (int i = 0; i < buttonLength; i++)
    {
        if (buttons[i].IsPushed())
        {
            action::Function action = IndexToAction(i);
            Robot& robot = GetRobot();
            robot.StoreBackup();
            if (actionEditor::selectedAction == -1)
            {
                if (!robot.AddAction(action))
                {
                    SDL_ShowSimpleMessageBox(
                        SDL_MESSAGEBOX_ERROR,
                        game::title, "Couldn't add action", game::window
                    );
                }
            }
            else robot.code.actions[actionEditor::selectedAction] = action;
            break;
        }
    }
}

static void addAction::Render(void)
{
    canvas::SetColor(COLOR_WHITE);
    canvas::DrawText(
        title,
        FONT_MAIN_PATH,
        transform.position + Vector2i(transform.size.x / 2, 0),
        GetTitleHeight(),
        canvas::TextAlign::TOP_MIDDLE
    );
    for (int i = 0; i < buttonLength; i++)
        addAction::buttons[i].Render();
}

static action::Function addAction::IndexToAction(int index)
{
    switch (index)
    {
    case 0:  return action::Exit;
    case 1:  return action::MoveForward;
    case 2:  return action::TurnRight;
    case 3:  return action::GoTo;
    case 4:  return action::GoToValue;
    case 5:  return action::ChangeValueTo;
    case 6:  return action::ChangeValueBy;
    case 7:  return action::MovePointerTo;
    case 8:  return action::MovePointerBy;
    case 9:  return action::CopyValueFrom;
    case 10: return action::IfThen;
    case 11: return action::IfNotThen;
    }
    return NULL;
}

static int addAction::GetTitleHeight(void)
{
    return GetActionHeight() * 2;
}

static void simpleEditAction::Update(void)
{
    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        if ((i + 1) * GetActionHeight() > codeUi::transform.size.y)
            break;
        Vector2i position = codeUi::transform.position + Vector2i(codeUi::transform.size.x, i * GetActionHeight()) - Vector2i(GetActionHeight() * 3, 0) - Vector2i(scrollBar::GetBarWidth(), 0);
        Vector2i size = Vector2i(GetActionHeight() * 3, GetActionHeight());
        shape::Rectangle trans(position, size);
        buttonsArray[i].Update(trans, i + startPoint);
    }
}

static void simpleEditAction::Render(void)
{
    for (int i = 0; i < MAX_CODE_LENGTH; i++)
    {
        if ((i + 1) * GetActionHeight() > codeUi::transform.size.y)
            break;
        buttonsArray[i].Render();
    }
}

static void simpleEditAction::CreateImages(void)
{
    buttonImage.Create(IMAGE_SIMPLE_ACTION_EDIT_PATH);
}

static void simpleEditAction::DestroyImages(void)
{
    buttonImage.Destroy();
}

void simpleEditAction::Buttons::Update(const shape::Rectangle& transform, int index)
{
    Vector2i size(transform.size.x / 3, transform.size.y);
    for (int i = 0; i < BUTTON_LENGTH; i++)
    {
        buttonPtrs[i]->Init(&buttonImage, transform.position + Vector2i(size.x * i, 0), size, 0);
        buttonPtrs[i]->Update();
        if (up.IsPushed()) UpAction(index);
        else if (down.IsPushed()) DownAction(index);
        else if (destroy.IsPushed()) DestroyAction(index);
    }
}

void simpleEditAction::Buttons::UpAction(int index)
{
    if (index == 0) return;
    GetRobot().StoreBackup();
    GetRobot().SwapAction(index, index - 1);
}

void simpleEditAction::Buttons::DownAction(int index)
{
    if (index == MAX_CODE_LENGTH - 1) return;
    GetRobot().StoreBackup();
    GetRobot().SwapAction(index, index + 1);
}

void simpleEditAction::Buttons::DestroyAction(int index)
{
    GetRobot().StoreBackup();
    GetRobot().DeleteAction(index);
}

void simpleEditAction::Buttons::Render(void) const
{
    Vector2i size(IMAGE_SIMPLE_ACTION_EDIT_SIZE);
    shape::Rectangle recs[BUTTON_LENGTH * 3];
    for (int i = 0; i < BUTTON_LENGTH * 3; i++)
        recs[i] = shape::Rectangle(Vector2i((i / BUTTON_LENGTH) * size.x, (i % 3) * size.y), size);
    for (int i = 0; i < BUTTON_LENGTH; i++)
        buttonPtrs[i]->Render(recs[i * BUTTON_LENGTH], recs[i * BUTTON_LENGTH + 1], recs[i * BUTTON_LENGTH + 2]);
}

static void actionEditor::Init(void)
{
    selectedAction = -1;
}

static void actionEditor::Update(void)
{
    Vector2i position = codeUi::transform.position + Vector2i(codeUi::transform.size.x, 0);
    Vector2i size = codeUi::transform.size;
    transform = shape::Rectangle(position, size);

    Vector2i mousePos = mouse::GetPosition();
    canvas::AdjustToWindow(mousePos);
    if ((shape::IsTriggered(codeUi::transform, mousePos) || shape::IsTriggered(addAction::transform, mousePos) || shape::IsTriggered(transform, mousePos)) && mouse::IsClicked(1))
    {
        if (shape::IsTriggered(codeUi::transform.AddedSize(Vector2i(-scrollBar::GetBarWidth(), 0)), mousePos))
        {
            int offset = mousePos.y - codeUi::transform.position.y;
            int index = offset / GetActionHeight() + startPoint;
            if (index <= MAX_CODE_LENGTH && index >= 0) selectedAction = index;
            else selectedAction = -1;
        }
    }
    else if (mouse::IsClicked(1)) selectedAction = -1;

    if (selectedAction == -1)
        return;

    isWithArg = false;
    for (int i = 0; i < WITH_ARG_FUNCS_LENGTH; i++)
    {
        if (withArgFuncs[i] == GetRobot().code.actions[selectedAction])
        {
            isWithArg = true;
            break;
        }
    }

    Vector2i buttonPosition = transform.position + Vector2i(0, GetTitleHeight());
    Vector2i buttonSize = Vector2i(transform.size.x, GetActionHeight());
    packAction.Init(buttonPosition, buttonSize, "Pack", BUTTON_BORDER);
    buttonPosition += Vector2i(0, GetActionHeight());
    deleteAndPackAction.Init(buttonPosition, buttonSize, "Delete and Pack", BUTTON_BORDER);
    buttonPosition += Vector2i(0, GetActionHeight());
    clearAll.Init(buttonPosition, buttonSize, "Clear All", BUTTON_BORDER);
    buttonPosition += Vector2i(0, GetActionHeight());
    undo.Init(buttonPosition, buttonSize, "Undo", BUTTON_BORDER);
    if (isWithArg)
    {
        incrementArg.Init(
            transform.position + Vector2i(0, GetActionHeight() * 5) + Vector2i(0, GetTitleHeight()),
            Vector2i(transform.size.x / 2, GetActionHeight()),
            "Increment Arguement",
            BUTTON_BORDER
        );
        decrementArg.Init(
            transform.position + Vector2i(0, GetActionHeight() * 5) + Vector2i(0, GetTitleHeight()) + Vector2i(transform.size.x / 2, 0),
            Vector2i(transform.size.x / 2, GetActionHeight()),
            "Decrement Arguement",
            BUTTON_BORDER
        );
    }
    for (int i = 0; i < BUTTON_LENGTH; i++)
        buttonPtrs[i]->Update();
    if (isWithArg)
    {
        incrementArg.Update();
        decrementArg.Update();
    }

    Robot& robot = GetRobot();

    if (packAction.IsPushed())
    {
        robot.StoreBackup();
        robot.PackAction(selectedAction);
    }
    else if (deleteAndPackAction.IsPushed())
    {
        robot.StoreBackup();
        robot.DeleteAction(selectedAction);
        robot.PackAction(selectedAction);
    }
    else if (clearAll.IsPushed())
    {
        robot.StoreBackup();
        for (int i = 0; i < MAX_CODE_LENGTH; i++)
            robot.DeleteAction(i);
    }
    else if (undo.IsPushed())
    {
        robot.RestoreBackup();
    }
    else if (incrementArg.IsPushed())
    {
        robot.StoreBackup();
        robot.code.args[selectedAction]++;
    }
    else if (decrementArg.IsPushed())
    {
        robot.StoreBackup();
        robot.code.args[selectedAction]--;
    }
}

static void actionEditor::Render(void)
{
    if (selectedAction == -1)
        return;

    canvas::SetColor(COLOR_WHITE);
    canvas::DrawText(
        title,
        FONT_MAIN_PATH,
        transform.position + Vector2i(transform.size.x / 2, 0),
        GetTitleHeight(),
        canvas::TextAlign::TOP_MIDDLE
    );

    if ((selectedAction - startPoint) * GetActionHeight() < codeUi::transform.position.y + codeUi::transform.size.y && (selectedAction - startPoint) * GetActionHeight() >= codeUi::transform.position.y)
    {
        canvas::SetColor(COLOR_YELLOW);
        Vector2i position = codeUi::transform.position + Vector2i(0, (selectedAction - startPoint) * GetActionHeight());
        Vector2i size = Vector2i(codeUi::transform.size.x, GetActionHeight()) - Vector2i(scrollBar::GetBarWidth(), 0);
        canvas::DrawRect(position, size);
        canvas::DrawRect(position + Vector2i(1), size + Vector2i(-2));
    }

    for (int i = 0; i < BUTTON_LENGTH; i++)
        buttonPtrs[i]->Render();
    if (isWithArg)
    {
        incrementArg.Render();
        decrementArg.Render();
    }
}

static int actionEditor::GetTitleHeight(void)
{
    return GetActionHeight() * 2;
}

} /* namespace codeUi */

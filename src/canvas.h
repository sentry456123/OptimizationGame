#ifndef CANVAS_H_
#define CANVAS_H_

#include "SDL.h"
#include "Vector2i.h"
#include "shape.h"

#define FONT_MAIN_PATH "res/consolas.ttf"

struct Color
{
    Uint8 r, g, b, a;
    Color(void);
    Color(Uint8 r, Uint8 g, Uint8 b);
    Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
};

namespace canvas
{
    enum class TextAlign
    {
        TOP_LEFT,
        TOP_MIDDLE,
        TOP_RIGHT,
        MIDDLE_LEFT,
        CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_MIDDLE,
        BOTTOM_RIGHT,
    };

    class Image
    {
    public:
        void Create(const char* filePath);
        void Destroy(void);
        SDL_Texture* GetTexture(void);
    protected:
        void Render(int fX, int fY, int fWidth, int fHeight, int iX, int iY, int iWidth, int iHeight, bool isFrameSet);
        SDL_Texture* texture;
    public:
        void Render(int fX, int fY, int fWidth, int fHeight, int iX, int iY, int iWidth, int iHeight);
        void Render(const Vector2i& fPos, const Vector2i& fSize, const Vector2i& iPos, const Vector2i& iSize);
        void Render(const shape::Rectangle& frame, const shape::Rectangle& image);
        void Render(int x, int y, int width, int height);
        void Render(const Vector2i& position, const Vector2i& size);
        void Render(const shape::Rectangle& transform);
    };

    void Init(int width, int height);
    void Update(void);
    void SetColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a);
    void SetColor(const Uint8 r, const Uint8 g, const Uint8 b);
    void SetColor(const Color&);
    void DrawRect(int x, int y, int width, int height);
    void DrawRect(const Vector2i& position, const Vector2i& size);
    void DrawRect(const shape::Rectangle&);
    void FillRect(int x, int y, int width, int height);
    void FillRect(const Vector2i& position, const Vector2i& size);
    void FillRect(const shape::Rectangle&);
    void DrawText(const char* text, const char* filePath, int x, int y, int size, TextAlign mode);
    void DrawText(const char* text, const char* filePath, const Vector2i& position, int size, TextAlign mode);

    void AdjustToCanvas(int* const x, int* const y);
    void AdjustToCanvas(Vector2i& position);
    void AdjustToWindow(int* const x, int* const y);
    void AdjustToWindow(Vector2i& position);

    extern int width, height;
}

#define COLOR_BLACK     Color(0x00, 0x00, 0x00)
#define COLOR_WHITE     Color(0xff, 0xff, 0xff)
#define COLOR_GREEN     Color(0x00, 0xff, 0x00)
#define COLOR_GRAY      Color(0x80, 0x80, 0x80)
#define COLOR_DARK_GRAY Color(0x19, 0x19, 0x19)
#define COLOR_RED       Color(0xff, 0x00, 0x00)
#define COLOR_YELLOW    Color(0xff, 0xff, 0x00)

#endif /* CANVAS_H_ */

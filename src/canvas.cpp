#include "canvas.h"
#include "game.h"
#include "SDL_ttf.h"
#include <iostream>
#include "SDL_image.h"

Color::Color(void) : r(0x00), g(0x00), b(0x00), a(0xff) {}

Color::Color(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b), a(0xff) {}

Color::Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {}

namespace canvas
{
	int x, y;
	int width, height;

	static int firstWidth, firstHeight;
	static float aspectRatio;
	static bool isWidthWider;
}

void canvas::AdjustToCanvas(int* const x, int* const y)
{
	if      ( isWidthWider && x) *x += canvas::x;
	else if (!isWidthWider && y) *y += canvas::y;
}

void canvas::AdjustToCanvas(Vector2i& position)
{
	AdjustToCanvas(&position.x, &position.y);
}

void canvas::AdjustToWindow(int* const x, int* const y)
{
	if      ( isWidthWider && x) *x -= canvas::x;
	else if (!isWidthWider && y) *y -= canvas::y;
}

void canvas::AdjustToWindow(Vector2i& position)
{
	AdjustToWindow(&position.x, &position.y);
}

void canvas::Init(int width, int height)
{
	firstWidth = width;
	firstHeight = height;
	canvas::width = width;
	canvas::height = height;
	aspectRatio = (float)width / (float)height;
}

void canvas::Update(void)
{
	float windowAspectRatio = (float)game::GetWindowSize().x / (float)game::GetWindowSize().y;
	isWidthWider = (aspectRatio < windowAspectRatio);

	if (isWidthWider)
	{
		height = game::GetWindowSize().y;
		width = (int)((float)height * aspectRatio);
	}
	else
	{
		width = game::GetWindowSize().x;
		height = (int)((float)width / aspectRatio);
	}

	x = (game::GetWindowSize().x - width)  / 2;
	y = (game::GetWindowSize().y - height) / 2;
}

void canvas::SetColor(const Uint8 r, const Uint8 g, const Uint8 b, const Uint8 a)
{
    SDL_SetRenderDrawColor(game::renderer, r, g, b, a);
}

void canvas::SetColor(const Uint8 r, const Uint8 g, const Uint8 b)
{
	SetColor(r, g, b, 0xff);
}

void canvas::SetColor(const Color& color)
{
	SetColor(color.r, color.g, color.b, color.a);
}

void canvas::DrawRect(int x, int y, int width, int height)
{
	AdjustToCanvas(&x, &y);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_RenderDrawRect(game::renderer, &rect);
}

void canvas::DrawRect(const Vector2i& position, const Vector2i& size)
{
    DrawRect(position.x, position.y, size.x, size.y);
}

void canvas::DrawRect(const shape::Rectangle& rectangle)
{
    DrawRect(rectangle.position, rectangle.size);
}

void canvas::FillRect(int x, int y, int width, int height)
{
	AdjustToCanvas(&x, &y);
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_RenderFillRect(game::renderer, &rect);
}

void canvas::FillRect(const Vector2i& position, const Vector2i& size)
{
    FillRect(position.x, position.y, size.x, size.y);
}

void canvas::FillRect(const shape::Rectangle& rectangle)
{
    FillRect(rectangle.position, rectangle.size);
}

void canvas::DrawText(const char* text, const char* filePath, int x, int y, int size, TextAlign mode)
{
	AdjustToCanvas(&x, &y);
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(game::renderer, &r, &g, &b, &a);
	SDL_Color sdlColor = { r, g, b, a };
	TTF_Font* font = TTF_OpenFont(filePath, size);
    if (!font)
    {
        std::cout << "Couldn't create font from " << filePath << std::endl;
    }
	SDL_Surface* surface = TTF_RenderText_Blended(font, text, sdlColor);
	if (!surface)
	{
		std::cout << "Couldn't create surface from " << filePath << std::endl;
		return;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(game::renderer, surface);
	if (!texture)
	{
		std::cout << "Couldn't create texture" << std::endl;
		return;
	}
	SDL_Rect rect;
	switch (mode)
	{
	case TextAlign::TOP_LEFT:      rect.x = x;                  rect.y = y;                  break;
	case TextAlign::TOP_MIDDLE:    rect.x = x - surface->w / 2; rect.y = y;                  break;
	case TextAlign::TOP_RIGHT:     rect.x = x - surface->w;     rect.y = y;                  break;
	case TextAlign::MIDDLE_LEFT:   rect.x = x;                  rect.y = y - surface->h / 2; break;
	case TextAlign::CENTER:        rect.x = x - surface->w / 2; rect.y = y - surface->h / 2; break;
	case TextAlign::MIDDLE_RIGHT:  rect.x = x - surface->w;     rect.y = y - surface->h / 2; break;
	case TextAlign::BOTTOM_LEFT:   rect.x = x;                  rect.y = y - surface->h;     break;
	case TextAlign::BOTTOM_MIDDLE: rect.x = x - surface->w / 2; rect.y = y - surface->h;     break;
	case TextAlign::BOTTOM_RIGHT:  rect.x = x - surface->w;     rect.y = y - surface->h;     break;
	}
	rect.w = surface->w;
	rect.h = surface->h;
	SDL_RenderCopy(game::renderer, texture, nullptr, &rect);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	TTF_CloseFont(font);
}

void canvas::DrawText(const char* text, const char* filePath, const Vector2i& position, int size, TextAlign mode)
{
    DrawText(text, filePath, position.x, position.y, size, mode);
}

void canvas::Image::Create(const char* filePath)
{
	SDL_Surface* surface = IMG_Load(filePath);
	if (!surface)
	{
		std::cout << "Couldn't create surface" << std::endl;
		return;
	}
	texture = SDL_CreateTextureFromSurface(game::renderer, surface);
	if (!texture)
	{
		std::cout << "Couldn't create texture" << std::endl;
		return;
	}
	SDL_FreeSurface(surface);
}

void canvas::Image::Destroy(void)
{
	SDL_DestroyTexture(texture);
}

SDL_Texture* canvas::Image::GetTexture()
{
	return texture;
}

void canvas::Image::Render(int fX, int fY, int fWidth, int fHeight, int iX, int iY, int iWidth, int iHeight, bool isFrameSet)
{
	AdjustToCanvas(&iX, &iY);
	SDL_Rect frameRect;
	frameRect.x = fX;
	frameRect.y = fY;
	frameRect.w = fWidth;
	frameRect.h = fHeight;
	SDL_Rect* frameRectPtr = isFrameSet ? &frameRect : NULL;

	SDL_Rect imageRect;
	imageRect.x = iX;
	imageRect.y = iY;
	imageRect.w = iWidth;
	imageRect.h = iHeight;

	SDL_RenderCopy(game::renderer, texture, frameRectPtr, &imageRect);
}

void canvas::Image::Render(int fX, int fY, int fWidth, int fHeight, int iX, int iY, int iWidth, int iHeight)
{
	Render(fX, fY, fWidth, fHeight, iX, iY, iWidth, iHeight, true);
}

void canvas::Image::Render(const Vector2i& fPos, const Vector2i& fSize, const Vector2i& iPos, const Vector2i& iSize)
{
	Render(fPos.x, fPos.y, fSize.x, fSize.y, iPos.x, iPos.y, iSize.x, iSize.y);
}

void canvas::Image::Render(const shape::Rectangle& frame, const shape::Rectangle& image)
{
	Render(frame.position, frame.size, image.position, image.size);
}

void canvas::Image::Render(int x, int y, int width, int height)
{
	Render(0, 0, 0, 0, x, y, width, height, false);
}

void canvas::Image::Render(const Vector2i& position, const Vector2i& size)
{
	Render(position.x, position.y, size.x, size.y);
}

void canvas::Image::Render(const shape::Rectangle& transform)
{
	Render(transform.position, transform.size);
}

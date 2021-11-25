#include "Button.h"
#include "Game.h"
#include "mouse.h"
#include <iostream>

void Button::Init(const Vector2i& position, const Vector2i& size, const char* text, const ButtonFlags flags)
{
	m_transform = shape::Rectangle(position, size);
	m_text = text;
	m_flags = flags;
}

void Button::Update(void)
{
	Vector2i mousePos = mouse::GetPosition();
	canvas::AdjustToWindow(mousePos);
	m_targeted = shape::IsTriggered(m_transform, mousePos);
	m_targeted ? m_touchedTime++ : m_touchedTime = 0;
	m_pushed = m_targeted && mouse::IsClicked(1);
}

void Button::Render(void) const
{
    using namespace canvas;
	Vector2i mousePos = mouse::GetPosition();
	AdjustToWindow(mousePos);
	int x =      m_transform.position.x;
	int y =      m_transform.position.y;
	int width =  m_transform.size.x;
	int height = m_transform.size.y;

    SetColor(COLOR_WHITE);
	if (m_flags & BUTTON_BORDER) DrawRect(m_transform);

	m_targeted ? SetColor(COLOR_DARK_GRAY) : SetColor(COLOR_BLACK);
	FillRect(x + 1, y + 1, width - 2, height - 2);

    SetColor(COLOR_WHITE);
	bool isPushed = shape::IsTriggered(m_transform, mousePos) && mouse::IsPressed(1);
    float pushDepth = isPushed ? (20.0f / 21.0f) : 1;
	float sizeRatio = (m_flags & BUTTON_UPPER_CASE) ? 1.0f : (2.0f / 3.0f);
    DrawText(m_text, FONT_MAIN_PATH, x + width / 2, y + height / 2, height * sizeRatio * pushDepth, TextAlign::CENTER);
}

bool Button::IsPushed(void)
{
	return m_pushed;
}

void ImageButton::Init(canvas::Image* image, const Vector2i& position, const Vector2i& size, const ButtonFlags flags)
{
	m_image = image;
	m_transform = shape::Rectangle(Vector2i(position), Vector2i(size));
	m_flags = flags;
}

/**
 *	@param normal   the part of the image usage when the button is not targeted nor pushed
 *	@param targeted the part of the image usage when the button is targeted
 *	@param pushed   the part of the image usage when the button is pushed
 */
void ImageButton::Render(const shape::Rectangle& normal, const shape::Rectangle& targeted, const shape::Rectangle& pushed) const
{
	bool isPushed = shape::IsTriggered(m_transform, mouse::GetPosition()) && mouse::IsPressed(1);
	if      (isPushed)   m_image->Render(pushed, m_transform);
	else if (m_targeted) m_image->Render(targeted, m_transform);
	else                 m_image->Render(normal, m_transform);
}

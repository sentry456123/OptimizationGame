#ifndef BUTTON_H_
#define BUTTON_H_

#include "Vector2i.h"
#include "shape.h"
#include <cstdint>
#include "canvas.h"

typedef int ButtonFlags;

#define BUTTON_DEFAULT    ButtonFlags(0x00000000)
#define BUTTON_UPPER_CASE ButtonFlags(0x00000001)
#define BUTTON_BORDER     ButtonFlags(0x00000002)

class Button
{
public:
	void Init(const Vector2i& position, const Vector2i& size, const char* text, const ButtonFlags flags);
	void Update(void);
	void Render(void) const;
	bool IsPushed(void);
protected:
	shape::Rectangle m_transform;
	const char* m_text;
	ButtonFlags m_flags;
	bool m_targeted;
	bool m_pushed;
	uint32_t m_touchedTime;
};

class ImageButton : public Button
{
public:
	void Init(const Vector2i& position, const Vector2i& size, const char* text, const ButtonFlags flags) = delete;
	void Init(canvas::Image* m_image, const Vector2i& position, const Vector2i& size, const ButtonFlags flags);
	void Render(const shape::Rectangle& normal, const shape::Rectangle& targeted, const shape::Rectangle& pushed) const;
protected:
	canvas::Image* m_image;
};



#endif /* BUTTON_H_ */

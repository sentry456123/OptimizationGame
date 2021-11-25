#include "shape.h"

shape::Rectangle::Rectangle(void) {}

shape::Rectangle::Rectangle(const Vector2i& position, const Vector2i& size)
{
    this->position = position;
    this->size = size;
}

shape::Rectangle shape::Rectangle::AddedPosition(const Vector2i& position)
{
	Rectangle buffer = *this;
	buffer.position += position;
	return buffer;
}

shape::Rectangle shape::Rectangle::AddedSize(const Vector2i& size)
{
	Rectangle buffer = *this;
	buffer.size += size;
	return buffer;
}

bool shape::IsTriggered(const Rectangle& rectangle, const Vector2i& point)
{
	if (rectangle.position.x >= point.x)
		return false;
	if (rectangle.position.x + rectangle.size.x < point.x)
		return false;
	if (rectangle.position.y >= point.y)
		return false;
	if (rectangle.position.y + rectangle.size.y < point.y)
		return false;
	return true;
}

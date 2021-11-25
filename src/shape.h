#ifndef SHAPE_H_
#define SHAPE_H_

#include "Vector2i.h"

namespace shape
{
    class Rectangle
    {
    public:
        Vector2i position, size;
        Rectangle(void);
        Rectangle(const Vector2i& position, const Vector2i& size);

        Rectangle AddedPosition(const Vector2i& position);
        Rectangle AddedSize(const Vector2i& size);
    };

    bool IsTriggered(const Rectangle& rectangle, const Vector2i& point);
}

#endif /* SHAPE_H_ */

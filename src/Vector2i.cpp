#include "Vector2i.h"
#include "MapLevel.h"

Vector2i::Vector2i(void) : x(0), y(0) {}

Vector2i::Vector2i(int x, int y) : x(x), y(y) {}

Vector2i::Vector2i(int xy) : x(xy), y(xy) {}

int Vector2i::ToCoord(void) const
{
    return y * MAP_SIZE + x;
}

Vector2i CoordToVector2i(int coord)
{
    Vector2i result;
    result.x = coord % MAP_SIZE;
    result.y = coord / MAP_SIZE;
    return result;
}

Vector2i operator+(const Vector2i& vec1, const Vector2i& vec2)
{
    return Vector2i(vec1.x + vec2.x, vec1.y + vec2.y);
}

Vector2i operator+=(Vector2i& vec1, const Vector2i& vec2)
{
    vec1 = vec1 + vec2;
    return vec1;
}

Vector2i operator-(const Vector2i& vec1, const Vector2i& vec2)
{
    return Vector2i(vec1.x - vec2.x, vec1.y - vec2.y);
}

Vector2i operator*(const Vector2i& vec1, const Vector2i& vec2)
{
    return Vector2i(vec1.x * vec2.x, vec1.y * vec2.y);
}

Vector2i operator*(const Vector2i& vec, float scalar)
{
    return Vector2i((int)((float)vec.x * scalar), (int)((float)vec.y * scalar));
}

Vector2i operator*(float scalar, const Vector2i& vec)
{
    return vec * scalar;
}

Vector2i operator/(const Vector2i& vec1, const Vector2i& vec2)
{
    return Vector2i(vec1.x / vec2.x, vec1.y / vec2.y);
}

Vector2i operator/(const Vector2i& vec, float scalar)
{
    return Vector2i((int)((float)vec.x / scalar), (int)((float)vec.y / scalar));
}

Vector2i operator/(float scalar, const Vector2i& vec)
{
    return Vector2i((int)(scalar / (float)vec.x), (int)(scalar / (float)vec.y));
}

#ifndef VECTOR2I_H_
#define VECTOR2I_H_

class Vector2i
{
public:
    int x, y;

    Vector2i(void);
    Vector2i(int x, int y);
    Vector2i(int xy);

   int ToCoord(void) const;
};

Vector2i CoordToVector2i(int coord);

Vector2i operator+ (const Vector2i& vec1, const Vector2i& vec2);
Vector2i operator+=(Vector2i& vec1, const Vector2i& vec2);
Vector2i operator- (const Vector2i& vec1, const Vector2i& vec2);
Vector2i operator* (const Vector2i& vec1, const Vector2i& vec2);
Vector2i operator* (const Vector2i& vec, float scalar);
Vector2i operator* (float scalar, const Vector2i& vec);
Vector2i operator/ (const Vector2i& vec1, const Vector2i& vec2);
Vector2i operator/ (const Vector2i& vec, float scalar);
Vector2i operator/ (float scalar, const Vector2i& vec);

#endif /* VECTOR2I_H_ */

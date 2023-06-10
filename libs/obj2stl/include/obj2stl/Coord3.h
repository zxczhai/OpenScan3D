#pragma once

#include <cmath>
#include <cfloat>

struct Coord3
{
    float x;
    float y;
    float z;

    Coord3(float x, float y, float z) : x(x), y(y), z(z) {}
    Coord3() : Coord3(0., 0., 0.) {}

    Coord3 operator+(const Coord3& other) const { return {x + other.x, y + other.y, z + other.z}; }
    Coord3 operator-(const Coord3& other) const { return {x - other.x, y - other.y, z - other.z}; }
    Coord3 operator*(float f) const { return {x * f, y * f, z * f}; }
    Coord3 operator/(float f) const { return {x / f, y / f, z / f}; }

    Coord3 Normalize() const
    {
        float length = GetLength();
        return length > FLT_MIN ? *this / length : Coord3();  // do not divide by zero
    }

    float GetLength() const
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    static Coord3 CrossProduct(const Coord3& a, const Coord3& b)
    {
        float x = (a.y * b.z) - (a.z * b.y);
        float y = (a.z * b.x) - (a.x * b.z);
        float z = (a.x * b.y) - (a.y * b.x);

        return {x, y, z};
    }
};

struct Coord3N
{
    Coord3N(const Coord3& vt, const Coord3& vn) : vt(vt), vn(vn) {}

    Coord3 vt;
    Coord3 vn;
};

struct Coord3TR
{
    Coord3TR(const Coord3& tr1, const Coord3& tr2, const Coord3& tr3) : c1(tr1), c2(tr2), c3(tr3) {}
    Coord3TR() : c1{ 1,0,0 }, c2{ 0,1,0 }, c3{ 0,0,1 } {}

    Coord3 c1;
    Coord3 c2;
    Coord3 c3;

    Coord3 ApplyTransformation(const Coord3& p) const
    {
        float x = p.x * c1.x + p.y * c2.x + p.z * c3.x;
        float y = p.x * c1.y + p.y * c2.y + p.z * c3.y;
        float z = p.x * c1.z + p.y * c2.z + p.z * c3.z;

        return { x, y, z };
    }
};

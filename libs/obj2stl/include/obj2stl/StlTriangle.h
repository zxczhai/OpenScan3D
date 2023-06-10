#pragma once

#include "Coord3.h"

struct StlTriangle
{
    Coord3 v1;
    Coord3 v2;
    Coord3 v3;
    Coord3 norm;

    StlTriangle(const Coord3& v1, const Coord3& v2, const Coord3& v3, const Coord3& norm = Coord3())
        : v1(v1), v2(v2), v3(v3), norm(norm) {}
};

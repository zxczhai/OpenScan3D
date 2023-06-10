#pragma once

#include "ObjModel.h"
#include "StlModel.h"

#include <vector>
#include <array>

class Converter
{
public:
    Converter(Coord3TR trMatix) : trMatix_(trMatix) {}
    void Convert(const ObjModel& objModel, StlModel& stlModel);

private:
    std::vector<std::vector<Coord3N>> polygons_;    // list of faces -> list of coordinate pairs (vertex, norm)
    std::vector<std::array<Coord3,3>> triangles_;  // list of faces -> 3 * coordinates

    Coord3TR trMatix_;

    void CreatePolygonsWithNorms(const std::vector<FaceVertex>& faceVertices, const std::vector<Coord3>& objVertices, const std::vector<Coord3>& objNorms);
    void CreateTrianglesWithNormsAndTransform(const std::vector<Coord3N>& poly, const Coord3& translate);
};




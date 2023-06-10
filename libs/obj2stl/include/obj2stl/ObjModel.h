#pragma once

#include <vector>

#include "Coord3.h"
#include "FaceVertex.h"

class ObjModel
{
public:
    const std::vector<Coord3>& GetVertices() const { return vertices_; }
    const std::vector<Coord3>& GetNorms() const { return norms_; }
    const std::vector<std::vector<FaceVertex>>& GetFaces() const { return faces_; }

    const Coord3& GetCoordMin() const { return coordMin_; }

    void AddVertex(const Coord3& v);
    void AddNorm(const Coord3& n);
    void AddFace(const std::vector<FaceVertex>& face);

private:
    std::vector<Coord3> vertices_;
    std::vector<Coord3> norms_;
    std::vector<std::vector<FaceVertex>> faces_;

    Coord3 coordMin_;
};

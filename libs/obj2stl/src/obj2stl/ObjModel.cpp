#include "ObjModel.h"
#include "FaceVertex.h"

#include <stdexcept>


void ObjModel::AddVertex(const Coord3& v)
{
    vertices_.push_back(v);
    
    if (v.x < coordMin_.x) coordMin_.x = v.x;
    if (v.y < coordMin_.y) coordMin_.y = v.y;
    if (v.z < coordMin_.z) coordMin_.z = v.z; 
}

void ObjModel::AddNorm(const Coord3& n)
{
    norms_.push_back(n);
}

void ObjModel::AddFace(const std::vector<FaceVertex>& face)
{
    for (auto const& p : face)
    {
        if (p.vIndex() < 0 || p.vIndex() > vertices_.size())
        {
            throw std::runtime_error("Vertex index is out of bounds");
        }
        if (p.vnIndex() < 0 || p.vnIndex() > norms_.size())
        {
            throw std::runtime_error("Normal index is out of bounds");
        }
    }
    faces_.push_back(face);
}

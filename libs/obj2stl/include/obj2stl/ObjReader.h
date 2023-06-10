#pragma once

#include "ObjModel.h"

#include <string>
#include <vector>
#include <istream>

class ObjReader
{
public:
    ObjReader(ObjModel& model) : model_(model) {}

    void ReadFromFile(const std::string& fname);
    void ReadFromStream(std::istream& is);

private:
    ObjModel& model_;

    void ParseLine(const std::string& line);

    void AddVertex(float x, float y, float z) const;
    void AddVertexTexture(float u, float v);
    void AddVertexNorm(float i, float j, float k);
    void AddFace(const std::vector<FaceVertex>& faceVertexIndices);
};



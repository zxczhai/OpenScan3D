#pragma once

struct FaceVertex
{
    FaceVertex() : indices_{ 0,0,0 } {}

    int& vIndex() { return indices_[0]; }
    int& vtIndex() { return indices_[1]; }
    int& vnIndex() { return indices_[2]; }

    int vIndex() const { return indices_[0]; }
    int vtIndex() const { return indices_[1]; }
    int vnIndex() const { return indices_[2]; }

    int* Indices() { return indices_; }

private:
    int  indices_[3];
};

#include "StlWriter.h"

#include <fstream>


void StlWriter::WriteToFile(const std::string& fname) const
{
    std::ofstream ofs(fname, std::ostream::binary);
    if (ofs.is_open())
    {
        WriteToStream(ofs);
        ofs.close();
        return;
    }
    throw new std::runtime_error("Cannot open file: " + fname);
}

void StlWriter::WriteToStream(std::ostream& os) const
{
    WriteHeader(os);
    WriteBody(os);
}

void StlWriter::WriteHeader(std::ostream& os) const
{
    static const char header[80] = { 0 };
    os.write(header, sizeof(header));

    uint32_t ntriangles = model_.GetTriangles().size();
    os.write(reinterpret_cast<const char *>(&ntriangles), sizeof(uint32_t));
}

static void WriteCoord(std::ostream& os, const Coord3& coord);

void StlWriter::WriteBody(std::ostream& os) const
{
    static const char attribute[2] = {0};

    for (auto tr : model_.GetTriangles())
    {
        WriteCoord(os, tr.norm);
        WriteCoord(os, tr.v1);
        WriteCoord(os, tr.v2);
        WriteCoord(os, tr.v3);
        os.write(attribute, sizeof(attribute));
    }
}

void WriteCoord(std::ostream& os, const Coord3& coord)
{
    os.write(reinterpret_cast<const char*>(&coord.x), sizeof(float));
    os.write(reinterpret_cast<const char*>(&coord.y), sizeof(float));
    os.write(reinterpret_cast<const char*>(&coord.z), sizeof(float));
}
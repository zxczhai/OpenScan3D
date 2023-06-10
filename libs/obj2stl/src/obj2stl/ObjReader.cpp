#include "ObjReader.h"
#include "FaceVertex.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <stdexcept>

void ObjReader::ReadFromFile(const std::string& fname)
{
    std::ifstream file(fname);
    if (file.is_open())
    {
        ReadFromStream(file);
        file.close();
        return;
    }
    throw std::runtime_error("Cannot open file: " + fname);
}

void ObjReader::ReadFromStream(std::istream& is)
{
    std::string line;
    while (getline(is, line))
    {
        ParseLine(line);
        //std::cout << line << "\n";
    }
}

void ObjReader::ParseLine(const std::string& line)
{
    static const std::string rxCoordStr = "([-+]?[0-9]*.?[0-9]+(?:[eE][-+]?[0-9]+)?)";
    static const std::regex rxV = std::regex("^(?:\\s*)" + rxCoordStr + "(?:\\s+)" + rxCoordStr + "(?:\\s+)" + rxCoordStr + "(?:.*)");
    static const std::regex rxVT = std::regex("^(?:\\s*)" + rxCoordStr + "(?:\\s+)" + rxCoordStr + "(?:.*)");
    static const std::regex rxVN = std::regex("^(?:\\s*)" + rxCoordStr + "(?:\\s+)" + rxCoordStr + "(?:\\s+)" + rxCoordStr + "(?:.*)");
    static const std::regex rxF = std::regex("(?:\\s*)((?:[-+]?[0-9]+)(?:/[-+]?[0-9]*)*)");  // 'v_ind' or 'v_ind/vt_ind' or 'v_ind/vt_ind/vn_ind' or 'v_ind//vn_ind'
    static const std::regex rxPrefix = std::regex("^(?:((?:\\s*)(f|v(?:n|t)?))\\s.*)"); // g1:prefix, g2:f|v|vn|vt

    std::smatch sm;
    if (std::regex_match(line, sm, rxPrefix))
    {
        if (sm[2].str() == "v" && std::regex_match(line.begin() + sm[1].length(), line.end(), sm, rxV))
        {
            AddVertex(std::stof(sm[1]), std::stof(sm[2]), std::stof(sm[3]));
        }
        else if (sm[2].str() == "vt" && std::regex_match(line.begin() + sm[1].length(), line.end(), sm, rxVT))
        {
            AddVertexTexture(std::stof(sm[1]), std::stof(sm[2]));
        }
        else if (sm[2].str() == "vn" && std::regex_match(line.begin() + sm[1].length(), line.end(), sm, rxVN))
        {
            AddVertexNorm(std::stof(sm[1]), std::stof(sm[2]), std::stof(sm[3]));
        }
        else if (sm[2].str() == "f")
        {
            std::vector<FaceVertex> faceVertices;

            std::string s(line.begin() + sm[1].length(), line.end());
            while (std::regex_search(s, sm, rxF))
            {
                FaceVertex faceVertex;
                int* vi = faceVertex.Indices(), *pvi = vi;

                std::istringstream tokenStream(sm[1].str());
                std::string token;
                while (std::getline(tokenStream, token, '/'))
                {
                    if (token.length() > 0)
                    {
                        *pvi = std::stoi(token);
                        if (*pvi < 0) // relative vertext indices
                        {
                            *pvi += static_cast<int>(pvi - vi == 0 ? model_.GetVertices().size() : pvi - vi == 2 ? model_.GetNorms().size() : 0) + 1;
                        }
                    }
                    if (++pvi - vi > 3) // check for overrun
                    {
                        std::cerr << "warn: face vertex contains more than 3 elements";
                        break;
                    }
                }

                faceVertices.push_back(faceVertex);
                s = sm.suffix().str();
            }

            AddFace(faceVertices);
        }
    }
}

void ObjReader::AddVertex(float x, float y, float z) const
{
    model_.AddVertex(Coord3(x, y, z));
}

void ObjReader::AddVertexTexture(float u, float v)
{    
    // TODO: not used
}

void ObjReader::AddVertexNorm(float i, float j, float k)
{
    model_.AddNorm(Coord3(i, j, k));
}

void ObjReader::AddFace(const std::vector<FaceVertex>& faceVertexIndices)
{
    model_.AddFace(faceVertexIndices);
}


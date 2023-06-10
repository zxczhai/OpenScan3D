#include "ObjReader.h"
#include "StlWriter.h"
#include "Converter.h"

#include <iostream>
#include <exception>
#include <sstream>
#include <vector>
#include <cstring>

static bool verbose_flag = false;

void read(ObjModel& model, const std::string& fname)
{
    ObjReader reader(model);
    reader.ReadFromFile(fname);

    std::cerr << "OBJ vertex count: " << model.GetVertices().size() << "\n";
    std::cerr << "OBJ normal count: " << model.GetNorms().size() << "\n";
    std::cerr << "OBJ face count: " << model.GetFaces().size() << "\n"; 
}

void write(const StlModel& model, const std::string& fname)
{
    std::cerr << "STL triangle count: " << model.GetTriangles().size() << "\n";

    StlWriter writer(model);
    if (fname == "-")
        writer.WriteToStream(std::cout);
    else
        writer.WriteToFile(fname);
}

void convert(const ObjModel& modelIn, StlModel& modelOut, const Coord3TR& tr)
{
    Converter converter(tr);
    converter.Convert(modelIn, modelOut);
}

void parsearg(int argc, char * const argv[], std::string& filein, std::string& fileout, Coord3TR& tr)
{
    if (argc < 2)
    {
        throw std::runtime_error("");
    }

    filein = argv[1];

    for (int i = 2; i < argc; ++i)
    {
        if (strncmp(argv[i], "-o", 2) == 0 && argc > i + 1)
        {
            fileout = argv[++i];
            continue;
        }

        if (strncmp(argv[i], "-t", 2) == 0 && argc > i + 1)
        {
            float* tref[9] =
            {
                &tr.c1.x, &tr.c1.y, &tr.c1.z,
                &tr.c2.x, &tr.c2.y, &tr.c2.z,
                &tr.c3.x, &tr.c3.y, &tr.c3.z
            };
            float** ptref = tref;

            std::istringstream tokenStream1(argv[++i]);
            std::string token1;
            while (std::getline(tokenStream1, token1, ';'))
            {
                std::istringstream tokenStream2(token1);
                std::string token2;
                while (std::getline(tokenStream2, token2, ','))
                {
                    **ptref++ = std::stof(token2);
                }
            }
            continue;
        }

        throw std::runtime_error(std::string("Unknown parameter ") + argv[i]);
    }
}

int main(int argc, char * const argv[]) 
{
    Coord3TR tr;
    std::string filein;
    std::string fileout;

    try 
    {
        parsearg(argc, argv, filein, fileout, tr);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << "\n";
        std::cerr << "\nUsage:\n";
        std::cerr << argv[0] << " <OBJ-file> [-o STL-file] [-t \"1,0,0;0,1,0;0,0,1\"]\n";
        exit(EXIT_FAILURE);
    }
    
    if (fileout.empty())
    {
        fileout = filein.substr(0, filein.find_last_of('.')) + ".stl";
    }

    ObjModel objModel;
    StlModel stlModel;

    try {
        read(objModel, filein);
        convert(objModel, stlModel, tr);
        write(stlModel, fileout);
    }
    catch (const std::exception & e)
    {
        std::cerr << e.what() << "\n";
        return -1;
    }
    
    return 0;
}

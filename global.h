#pragma once
#include <QString>

using namespace std;

#define CMD_FULLAUTO 100
#define CMD_MATCHFEATURES 101
#define CMD_SFMANDSFP 102
#define CMD_EXPORTSPARSECLOUD 103
#define CMD_EXPORTDENSECLOUD 104
#define CMD_RECONSTRUCTMESH 105
#define CMD_TEXTUREMESH 106
#define CMD_IMPORTFROMBE 107
#define LISTIMAGES 1
#define COMPUTEFEATURES 2
#define MATCHFEATURES 3
#define SFM 4
#define SFP 5
#define COLORED 6
#define SPARSE 7
#define DENSE 8
#define DENSEFUSE 12
#define REMESH 9
#define REFINE 10
#define TEXTURE 11
#define FULLAUTO 13
#define PROCESSWORKING 0
#define PROCESSCLOSE 1
#define PROCESSERROR 2
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

class Global
{
public:
    static QString imagesInputDir;
    static QString matchesOutputDir;
    static QString sensorWidthDatabaseDir;
    static QString matchesDir;
    static QString sfmOutputDir;
    static QString densifyInputDir;
    static QString densifyOutputDir;
    static QString reconstructMeshInputDir;
    static QString reconstructMeshOutputDir;
    static QString textureMeshInputDir;
    static QString textureMeshOutputDir;
};


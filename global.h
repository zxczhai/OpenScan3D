#pragma once
#include <QDir>
#include <QString>
#include <QStringList>
#include <QTextStream>
#include <QProcess>
#include <QMap>


using namespace std;

#define CMD_FULLAUTO 100
#define CMD_MATCHFEATURES 101
#define CMD_SFMANDSFP 102
#define CMD_EXPORTSPARSECLOUD 103
#define CMD_EXPORTDENSECLOUD 104
#define CMD_RECONSTRUCTMESH 105
#define CMD_TEXTUREMESH 106
#define CMD_IMPORTFROMBE 107
#define CMD_IMPORTVIDEO 108
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
    static QString autoRebuildInPutDir;
    static QString autoRebuildOutputDir;
    static QString importVideoFramesInputDir;
    static QString importVideoFramesOutputDir;
    static QString videostarttime;
    static QString videoendtime;
    static QString eigenMatrix;
    static QString cameraModel;
    static QString imagesGroup;
    static QString sensorSize;
    static QStringList  cameraModels;
    static QMap<QString, double> cameraModelsWithSize;
    static bool isFilled;
    static int numberOfImages;
    static int engineTid;
    static void connectEngine();
    //static pid_t GetProcessIdFromName(const char* name);
    static pid_t GetProcessIdFromName(const QString& name);
    static int process;
    static int processProject;
    static int processState;
    static QString processETA;
    //static bool getProcessMsg();
    static bool tasking;
    static bool autoTasking;

};


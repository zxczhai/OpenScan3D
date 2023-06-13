#include "global.h"

QString Global::imagesInputDir = "";
QString Global::matchesOutputDir = "";
QString Global::sensorWidthDatabaseDir = "";
QString Global::matchesDir = "";
QString Global::sfmOutputDir = "";
QString Global::densifyInputDir = "";
QString Global::densifyOutputDir = "";
QString Global::reconstructMeshInputDir = "";
QString Global::reconstructMeshOutputDir = "";
QString Global::textureMeshInputDir = "";
QString Global::textureMeshOutputDir = "";
QString Global::autoRebuildInPutDir = "";
QString Global::autoRebuildOutputDir = "";
QString Global::importVideoFramesInputDir = "";
QString Global::importVideoFramesOutputDir = "";
QString Global::videostarttime = "";
QString Global::videoendtime = "";
QString Global::processETA = "";
QString Global::eigenMatrix = "";
QString Global::cameraModel = "vacant";
QString Global::imagesGroup = "vacant";
QString Global::sensorSize = "vacant";
QMap<QString, double> Global:: cameraModelsWithSize ={};
QStringList Global::cameraModels ={};
int Global::numberOfImages = 0;
bool Global::tasking = false;
bool Global::autoTasking = false;
bool Global::isFilled = false;
int Global::engineTid = 0;
int Global::process = PROCESSWORKING;
int Global::processProject = 0;
int Global::processState = 0;

void Global::connectEngine()
{
    // 检查是否存在tmp文件夹
    if (!QDir("/tmp").exists())
    {
        // 如果不存在就创建tmp目录
        QDir().mkpath("/tmp");
    }
    // 创建OpenScan3D目录
    QDir("/tmp").mkdir(".OpenScan3D");
    QFile file("/tmp/.OpenScan3D/ProgramCache.tmp");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return;
    }
    QString tid = QString(file.readAll());
    file.close();
    Global::engineTid = tid.toInt();
    if (Global::engineTid == 0)
    {
        return;
    }
    return;
}

//pid_t Global::GetProcessIdFromName(const char* name) {
//    pid_t pid = 0;
//    QDir dir("/proc");
//    QStringList list = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
//    for (const QString& entry : list) {
//        bool ok;
//        int id = entry.toInt(&ok);
//        if (!ok) continue;
//        QString path = QString("/proc/%1/status").arg(id);
//        QFile file(path);
//        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//            QTextStream in(&file);
//            while (!in.atEnd()) {
//                QString line = in.readLine();
//                if (line.startsWith("Name:")) {
//                    QString nameField = line.mid(5).trimmed();
//                    if (nameField.compare(name, Qt::CaseInsensitive) == 0) {
//                        pid = id;
//                        break;
//                    }
//                }
//            }
//            file.close();
//        }
//        if (pid > 0) break;
//    }
//    return pid;
//}

pid_t Global::GetProcessIdFromName(const QString& name) {
    pid_t pid = 0;
    QProcess process;
    QStringList arguments;
    arguments << "-x" << name;
#if defined(Q_OS_LINUX)
    process.start("pgrep", arguments);
//#elif defined(Q_OS_MACOS)
//    process.start("pgrep", arguments);
//#elif defined(Q_OS_WIN)
//    process.start("tasklist.exe", QStringList() << "/nh" << "/fi" << QString("imagename eq %1").arg(name));
#endif
    if (process.waitForFinished()) {
        QByteArray output = process.readAllStandardOutput();
        QList<QByteArray> lines = output.split('\n');
        if (lines.size() > 0) {
            bool ok;
            pid = lines.first().toInt(&ok);
        }
    }
    return pid;
}


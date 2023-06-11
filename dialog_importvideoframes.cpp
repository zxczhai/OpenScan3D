#include "dialog_importvideoframes.h"
#include "ui_dialog_importvideoframes.h"
#include "message.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <sys/stat.h> // For mkdir() function
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

Dialog_ImportVideoFrames::Dialog_ImportVideoFrames(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_ImportVideoFrames)
{
    ui->setupUi(this);
}

Dialog_ImportVideoFrames::~Dialog_ImportVideoFrames()
{
    delete ui;
}

void Dialog_ImportVideoFrames::extract_keyframes(const string& video_file, const string& output_dir)
{

    VideoCapture cap(video_file);
    if (!cap.isOpened())
    {
        cerr << "Error opening video file." << endl;
        return;
    }

    struct stat info;
    if (stat(output_dir.c_str(), &info) != 0)
    {
        int status = mkdir(output_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (status != 0)
        {
            cerr << "Error creating output directory." << endl;
            return;
        }
    }

    Mat prev_frame, prev_gray;
    cap >> prev_frame;
    if (prev_frame.empty())
    {
        cerr << "Error reading video file." << endl;
        return;
    }
    cvtColor(prev_frame, prev_gray, COLOR_BGR2GRAY);

    int frame_count = 0, keyframe_count = 0;
    const int max_keyframes = 100; // Maximum number of keyframes to extract
    const double min_disp = 4;  // Minimum median displacement for a keyframe
    const int total_frames = cap.get(CAP_PROP_FRAME_COUNT); // Total number of frames in the video
    const int interval = total_frames / max_keyframes; // Interval between keyframes

    while (true)
    {
        Mat frame, gray;
        cap >> frame;
        if (frame.empty())
            break;
        cvtColor(frame, gray, COLOR_BGR2GRAY);

        vector<Point2f> prev_pts, next_pts;
        const int max_corners = 500;
        const double quality_level = 0.01;
        const double min_distance = 10.0;
        const int block_size = 3;
        TermCriteria criteria = TermCriteria(TermCriteria::COUNT + TermCriteria::EPS, 30, 0.01);
        goodFeaturesToTrack(prev_gray, prev_pts, max_corners, quality_level, min_distance);
        vector<uchar> status;
        vector<float> error;
        calcOpticalFlowPyrLK(prev_gray, gray, prev_pts, next_pts, status, error, Size(block_size, block_size), 3, criteria);

        vector<double> displacements;
        for (decltype(prev_pts.size()) i = 0; i < prev_pts.size(); i++)
        {
            double dx = next_pts[i].x - prev_pts[i].x;
            double dy = next_pts[i].y - prev_pts[i].y;
            displacements.push_back(sqrt(dx*dx + dy*dy));
        }
        sort(displacements.begin(), displacements.end());
        double median_disp = displacements[displacements.size() / 2];
        if (median_disp >= min_disp && keyframe_count < max_keyframes && frame_count % interval == 0)
        {
            string filename = output_dir + "/keyframe_" + to_string(keyframe_count) + ".jpg";
            imwrite(filename, frame);
            cout << "Saved keyframe #" << keyframe_count << " with displacement " << median_disp << endl;
            keyframe_count++;
        }

        prev_frame = frame.clone();
        prev_gray = gray.clone();
        frame_count++;
    }
    cap.release();

}


void Dialog_ImportVideoFrames::on_pushButton_browseInputDir_clicked()
{
    Global::importVideoFramesInputDir = QFileDialog::getOpenFileName(this, tr("选择输入的视频文件"), QDir::homePath(), tr("All files(*.*)"));
    ui->lineEdit_inputDir->setText(Global::importVideoFramesInputDir);

    /*获取视频时长*/
    QString filePath = ui->lineEdit_inputDir->text();

    // 将QString类型的变量转换为std::string类型
    std::string path = filePath.toStdString();

    // 使用C-style字符串作为参数创建VideoCapture对象
    VideoCapture cap(path.c_str());

    // 检查是否成功打开了视频文件
    if (!cap.isOpened()) {
        // 如果无法打开视频文件，则显示警告对话框并清空lineEdit内容
        QMessageBox::warning(this, "错误", "打开视频文件文件失败！");
        ui->StartTime->clear();
        ui->EndTime->clear();
        ui->lineEdit_inputDir->clear();
        ui->lineEdit_OutputDir->clear();
        return;
    }
    // 获取视频信息
    double frameCount = cap.get(CAP_PROP_FRAME_COUNT);
    double fps = cap.get(CAP_PROP_FPS);
    double lengthInSeconds = frameCount / fps;
    // 检查视频长度是否大于0
    if (lengthInSeconds <= 0) {
        // 如果视频长度小于等于0，则将Start和End时间设置为0
        ui->StartTime->setText(QString::number(0.0, 'f', 2));
        ui->EndTime->setText(QString::number(0.0, 'f', 2));
    } else {
        // 如果视频长度大于0，则设置Start和End时间
        double start = 0.0; // 设置StartTime
        double end = lengthInSeconds; // 设置EndTime

        ui->StartTime->setText(QString::number(start, 'f', 2));
        ui->EndTime->setText(QString::number(end, 'f', 2));
    }

    /*在输入目录下自动创建一个新目录存储视频帧*/
    // 创建QFileInfo对象
    QFileInfo fileInfo(ui->lineEdit_inputDir->text());
    // 获取文件的绝对路径
    QString absolutePath = fileInfo.absolutePath(); // "/home/cjy/001"
    QDir dir(absolutePath);
    QString newDirName = "video_frame_extraction"; // 新文件夹的名称
    if (dir.exists(newDirName)) { // 如果指定目录中已经存在同名文件夹，则给新文件夹名称加后缀
        int i = 1;
        QString newName = QString("%1_%2").arg(newDirName).arg(i);
        while(dir.exists(newName)) {
            i++;
            newName = QString("%1_%2").arg(newDirName).arg(i);
        }
        newDirName = newName;
    }
    if (dir.mkdir(newDirName)) { // 在指定目录下创建新文件夹
        qDebug() << "新文件夹创建成功";
    } else {
        qDebug() << "新文件夹创建失败";
    }
    QString newDirPath = dir.absoluteFilePath(newDirName);
    ui->lineEdit_OutputDir->setText(newDirPath);

}

void Dialog_ImportVideoFrames::on_pushButton_browseOutputDir_clicked()
{
    Global::importVideoFramesOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出文件夹 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::importVideoFramesOutputDir);

    /*在输入目录下自动创建一个新目录存储视频帧*/
    QDir dir(ui->lineEdit_OutputDir->text());
    QString newDirName = "video_frame_extraction"; // 新文件夹的名称
    if (dir.exists(newDirName)) { // 如果指定目录中已经存在同名文件夹，则给新文件夹名称加后缀
        int i = 1;
        QString newName = QString("%1_%2").arg(newDirName).arg(i);
        while(dir.exists(newName)) {
            i++;
            newName = QString("%1_%2").arg(newDirName).arg(i);
        }
        newDirName = newName;
    }
    if (dir.mkdir(newDirName)) { // 在指定目录下创建新文件夹
        qDebug() << "新文件夹创建成功";
    } else {
        qDebug() << "新文件夹创建失败";
    }
    QString newDirPath = dir.absoluteFilePath(newDirName);
    ui->lineEdit_OutputDir->setText(newDirPath);
}

void Dialog_ImportVideoFrames::on_btn_CONFIRM_clicked()
{

    if (ui->lineEdit_inputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未选择输入的视频文件！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(ui->StartTime->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"开始时间不能为空！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if(ui->EndTime->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"结束时间不能为空！ ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未选择输出路径！", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //    if(!isDirectoryEmpty(ui->lineEdit_OutputDir->text()))
    //    {
    //        QMessageBox::critical(this, u8"错误 ", u8"该文件夹不为空，请重新选择！", QMessageBox::Ok, QMessageBox::Ok);
    //        ui->lineEdit_OutputDir->clear();
    //        return;
    //    }

    Global::importVideoFramesInputDir = ui->lineEdit_inputDir->text();

    Global::importVideoFramesOutputDir = ui->lineEdit_OutputDir->text();

    Global::videostarttime = ui->StartTime->text();

    Global::videoendtime = ui->EndTime->text();

    /*提取视频帧*/
    QString qstr = ui->lineEdit_inputDir->text();
    std::string str = qstr.toStdString();
    const std::string& constStrRef = str;

    QString qstr1 = ui->lineEdit_OutputDir->text();
    std::string str1 = qstr1.toStdString();
    const std::string& constStrRef1 = str1;

//    QMessageBox::information(this, "提示", "点击Ok开始提取视频帧,并等待");
        QMessageBox msgBox(QMessageBox::Information, "提示", "正在提取视频帧，请等待", QMessageBox::NoButton, this,
                           Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        msgBox.setWindowModality(Qt::ApplicationModal);
        msgBox.show();

    // 调用提取关键帧函数
    extract_keyframes(constStrRef,constStrRef1);

        msgBox.setText("视频帧提取完成");
        msgBox.exec();
//    QMessageBox::information(this, "提示", "视频帧提取完成");

    /*将参数写入文件*/
    // 检查是否存在tmp文件夹
    if (!QDir("/tmp").exists())
    {
        // 如果不存在就创建tmp目录
        QDir().mkpath("/tmp");
    }
    // 创建OpenScan3D目录
    QDir("/tmp").mkdir(".OpenScan3D");

    QFile cmdcache("/tmp/.OpenScan3D/cmdCache.tmp");

    if (cmdcache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
    {
        QString head = "importvideo\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(Global::importVideoFramesInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::videostarttime.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::videoendtime.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::importVideoFramesOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_IMPORTVIDEO;
        sendMessage(msg);
        Global::tasking = true;
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }

}

void Dialog_ImportVideoFrames::on_btn_CANCEL_clicked()
{
    this->close();
}

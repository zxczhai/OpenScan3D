#include "dialog_autorebuild.h"
#include "ui_dialog_autorebuild.h"
#include "message.hpp"
Dialog_AutoRebuild::Dialog_AutoRebuild(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_AutoRebuild)
{
    ui->setupUi(this);
}

Dialog_AutoRebuild::~Dialog_AutoRebuild()
{
    delete ui;
}

void Dialog_AutoRebuild::on_pushButton_browseOutputDir_clicked()
{
    Global::autoRebuildOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览结果输出文件夹", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::autoRebuildOutputDir);
}

void Dialog_AutoRebuild::on_btn_CONFIRM_clicked()
{
    QString describerMethod, quality, upright, forceCompute, geometricModel, distanceRatio, forceMatch, nearest_matching_method = "AUTO", sfmEngine, isRobustTriangulation;

    if (Global::GetProcessIdFromName("R3D") == 0)
    {
        QMessageBox::critical(this, u8"错误 ", u8"未找到R3D进程", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
        Global::connectEngine();

    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    Global::autoRebuildOutputDir = ui->lineEdit_OutputDir->text();

    switch (ui->comboBox_describer->currentIndex())
    {
    case 0:
    {
        describerMethod = "SIFT";
        break;
    }
    case 1:
    {
        describerMethod = "SIFT_ANATOMY";
        break;
    }
    case 2:
    {
        describerMethod = "AKAZE_FLOAT";
        break;
    }
    case 3:
    {
        describerMethod = "AKAZE_MLDB";
        break;
    }
    }

    switch (ui->comboBox_quality->currentIndex())
    {
    case 0:
    {
        quality = "NORMAL";
        break;
    }
    case 1:
    {
        quality = "HIGH";
        break;
    }
    case 2:
    {
        quality = "ULTRA";
        break;
    }
    }

    upright = QString::number(ui->comboBox_upright->currentIndex());

    forceCompute = QString::number(ui->comboBox_forcecompute->currentIndex());

    switch (ui->comboBox_geometricmodel->currentIndex())
    {
    case 0:
    {
        geometricModel = "f";
        break;
    }
    case 1:
    {
        geometricModel = "e";
        break;
    }
    case 2:
    {
        geometricModel = "h";
        break;
    }
    case 3:
    {
        geometricModel = "a";
        break;
    }
    case 4:
    {
        geometricModel = "u";
        break;
    }
    case 5:
    {
        geometricModel = "o";
        break;
    }
    }

    distanceRatio = ui->lineEdit_distanceRatio->text();

    forceMatch = QString::number(ui->comboBox_forcematch->currentIndex());

    switch (ui->comboBox_sfmEngine->currentIndex())
    {
    case 0: {
        sfmEngine = "INCREMENTAL";
        break;
    }
    case 1: {
        sfmEngine = "INCREMENTALV2";
        break;
    }
    case 2: {
        sfmEngine = "GLOBAL";
        break;
    }
    case 3: {
        sfmEngine = "STELLAR";
        break;
    }
    }

    switch (ui->comboBox_isRobustTriangulation->currentIndex())
    {
    case 0: {
        isRobustTriangulation = "on";
        break;
    }
    case 1: {
        isRobustTriangulation = "off";
        break;
    }
    }

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
        QString head = "CMD_FULLAUTO\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(Global::imagesInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::sensorWidthDatabaseDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::autoRebuildOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::eigenMatrix.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(describerMethod.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(quality.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(upright.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(forceCompute.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(distanceRatio.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(forceMatch.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(nearest_matching_method.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(geometricModel.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::autoRebuildOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::autoRebuildOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(sfmEngine.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(isRobustTriangulation.toUtf8());
        cmdcache.write("\n");
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_FULLAUTO;
        sendMessage(msg);
        Global::autoTasking = true;
        ui->lineEdit_OutputDir->clear();
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }

}

void Dialog_AutoRebuild::on_btn_CANCEL_clicked()
{
    this->close();
}

#include "dialog_featurematch.h"
#include "ui_dialog_featurematch.h"
#include "message.hpp"
Dialog_FeatureMatch::Dialog_FeatureMatch(QWidget *parent) : QDialog(parent),
                                                            ui(new Ui::Dialog_FeatureMatch)
{
    ui->setupUi(this);
}

Dialog_FeatureMatch::~Dialog_FeatureMatch()
{
    delete ui;
}

void Dialog_FeatureMatch::on_pushButton_browseInputDir_clicked()
{
    Global::imagesInputDir = QFileDialog::getExistingDirectory(this, u8"浏览图片文件夹 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::imagesInputDir);
}

void Dialog_FeatureMatch::on_pushButton_browseDatabaseDir_clicked()
{
    Global::sensorWidthDatabaseDir = QFileDialog::getOpenFileName(this, tr("选择相机数据库文件"), QDir::homePath(), tr("All files(*.*)"));
    ui->lineEdit_databaseDir->setText(Global::sensorWidthDatabaseDir);
}

void Dialog_FeatureMatch::on_pushButton_browseOutputDir_clicked()
{
    Global::matchesOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出文件夹", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::matchesOutputDir);
}

void Dialog_FeatureMatch::on_btn_CONFIRM_clicked()
{
    QString eigenMatrix, imagesInputDir, matchesOutputDir, sensorWidthDatabaseDir, describerMethod, quality, upright, forceCompute, geometricModel, distanceRatio, forceMatch, nearest_matching_method = "AUTO";

    if (ui->lineEdit_inputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入图片路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_databaseDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未选择相机数据库文件 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_eigenMatrix->text() == "")
    {
        if (QMessageBox::warning(this, u8"未输入本征矩阵 ", u8"忽略本征矩阵参数 ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
        {
            return;
        }
        else
            eigenMatrix = "NULL";
    }
    else
        eigenMatrix = ui->lineEdit_eigenMatrix->text();

    imagesInputDir = ui->lineEdit_inputDir->text();
    Global::imagesInputDir = imagesInputDir;

    matchesOutputDir = ui->lineEdit_OutputDir->text();
    Global::matchesOutputDir = matchesOutputDir;

    sensorWidthDatabaseDir = ui->lineEdit_databaseDir->text();
    Global::sensorWidthDatabaseDir = sensorWidthDatabaseDir;

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
        QString head = "matchfeature\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(imagesInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(sensorWidthDatabaseDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(matchesOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(eigenMatrix.toUtf8());
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
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_MATCHFEATURES;
        sendMessage(msg);
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }
}

void Dialog_FeatureMatch::on_btn_CANCEL_clicked()
{
    this->close();
}

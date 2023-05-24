#include "dialog_densifypointcloud.h"
#include "ui_dialog_densifypointcloud.h"
#include "message.hpp"
Dialog_DensifyPointCloud::Dialog_DensifyPointCloud(QWidget *parent) : QDialog(parent),
                                                                      ui(new Ui::Dialog_DensifyPointCloud)
{
    ui->setupUi(this);
}

Dialog_DensifyPointCloud::~Dialog_DensifyPointCloud()
{
    delete ui;
}

void Dialog_DensifyPointCloud::on_pushButton_browseInputDir_clicked()
{
    Global::densifyInputDir = QFileDialog::getExistingDirectory(this, u8"浏览重建结果文件夹 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::densifyInputDir);
}

void Dialog_DensifyPointCloud::on_pushButton_browseOutputDir_clicked()
{
    Global::densifyOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出点云文件目录 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::densifyOutputDir);
}

void Dialog_DensifyPointCloud::on_btn_CONFIRM_clicked()
{
    if (Global::GetProcessIdFromName("R3D") == 0)
    {
        QMessageBox::critical(this, u8"错误 ", u8"未找到R3D进程", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
        Global::connectEngine();

    if (ui->lineEdit_inputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入重建结果路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    Global::densifyInputDir = ui->lineEdit_inputDir->text();
    Global::densifyOutputDir = ui->lineEdit_OutputDir->text();

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
        QString head = "DENSIFYPOINTCLOUD\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(Global::densifyInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::densifyOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_EXPORTDENSECLOUD;
        sendMessage(msg);
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }
}

void Dialog_DensifyPointCloud::on_btn_CANCEL_clicked()
{
    this->close();
}

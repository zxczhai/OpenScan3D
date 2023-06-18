#include "dialog_texturemesh.h"
#include "ui_dialog_texturemesh.h"
#include "message.hpp"
#include <QProcess>
Dialog_TextureMesh::Dialog_TextureMesh(QWidget *parent) : QDialog(parent),
    ui(new Ui::Dialog_TextureMesh)
{
    ui->setupUi(this);
}

Dialog_TextureMesh::~Dialog_TextureMesh()
{
    delete ui;
}

void Dialog_TextureMesh::on_pushButton_browseInputDir_clicked()
{
    Global::textureMeshInputDir = QFileDialog::getExistingDirectory(this, u8"浏览模型重建文件目录 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::textureMeshInputDir);
}

void Dialog_TextureMesh::on_pushButton_browseOutputDir_clicked()
{
    Global::textureMeshOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出纹理模型文件目录 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::textureMeshOutputDir);
}

void Dialog_TextureMesh::on_btn_CONFIRM_clicked()
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
        QMessageBox::critical(this, u8"错误 ", u8"未输入密集点云结果路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    Global::textureMeshInputDir = ui->lineEdit_inputDir->text();
    Global::textureMeshOutputDir = ui->lineEdit_OutputDir->text();
    Global::originalPath = Global::textureMeshOutputDir;
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
        QString head = "TEXTUREMESH\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(Global::textureMeshInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::textureMeshOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_TEXTUREMESH;
        sendMessage(msg);
        Global::tasking = true;
        ui->lineEdit_inputDir->clear();
        ui->lineEdit_OutputDir->clear();
        this->close();
        // rcvMessage(msg);
        // QList<qint64> pidlist;
        // QStringList programs;
        // programs
        //     << "/usr/local/bin/OpenMVS/Viewer "+Global::textureMeshOutputDir.toUtf8();
        // foreach (QString pro, programs)
        // {
        //     QProcess process;
        //     qint64 pid;
        //     if (process.startDetached(pro, QStringList(), "", &pid))
        //     {
        //         pidlist.push_back(pid);
        //     }
        // }
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }
}

void Dialog_TextureMesh::on_btn_CANCEL_clicked()
{
    this->close();
}

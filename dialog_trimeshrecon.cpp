#include "dialog_trimeshrecon.h"
#include "ui_dialog_trimeshrecon.h"
#include "message.hpp"
Dialog_TriMeshRecon::Dialog_TriMeshRecon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_TriMeshRecon)
{
    ui->setupUi(this);
}

Dialog_TriMeshRecon::~Dialog_TriMeshRecon()
{
    delete ui;
}

void Dialog_TriMeshRecon::on_pushButton_browseInputDir_clicked()
{
    Global::reconstructMeshInputDir = QFileDialog::getExistingDirectory(this, u8"浏览密集点云文件目录 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::reconstructMeshInputDir);
}

void Dialog_TriMeshRecon::on_pushButton_browseOutputDir_clicked()
{
    Global::reconstructMeshOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出三角网模型文件目录 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::reconstructMeshOutputDir);
}

void Dialog_TriMeshRecon::on_btn_CONFIRM_clicked()
{
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

    Global::reconstructMeshInputDir = ui->lineEdit_inputDir->text();
    Global::reconstructMeshOutputDir = ui->lineEdit_OutputDir->text();

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
        QString head = "RECONSTRUCTMESH\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(Global::reconstructMeshInputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(Global::reconstructMeshOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Yes);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_RECONSTRUCTMESH;
        sendMessage(msg);
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
    }

}

void Dialog_TriMeshRecon::on_btn_CANCEL_clicked()
{
    this->close();
}

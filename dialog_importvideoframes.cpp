#include "dialog_importvideoframes.h"
#include "ui_dialog_importvideoframes.h"
#include "message.hpp"
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

bool Dialog_ImportVideoFrames::isDirectoryEmpty(QString path){
    QDir dir(path);
    return dir.isEmpty();
}

void Dialog_ImportVideoFrames::on_pushButton_browseInputDir_clicked()
{
    Global::importVideoFramesInputDir = QFileDialog::getOpenFileName(this, tr("选择输入的视频文件"), QDir::homePath(), tr("All files(*.*)"));
    ui->lineEdit_inputDir->setText(Global::importVideoFramesInputDir);
}

void Dialog_ImportVideoFrames::on_pushButton_browseOutputDir_clicked()
{
    Global::importVideoFramesOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览输出文件夹 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::importVideoFramesOutputDir);
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

    if(!isDirectoryEmpty(ui->lineEdit_OutputDir->text()))
    {
        QMessageBox::critical(this, u8"错误 ", u8"该文件夹不为空，请重新选择！", QMessageBox::Ok, QMessageBox::Ok);
        ui->lineEdit_OutputDir->clear();
        return;
    }

    Global::importVideoFramesInputDir = ui->lineEdit_inputDir->text();

    Global::importVideoFramesOutputDir = ui->lineEdit_OutputDir->text();

    Global::videostarttime = ui->StartTime->text();

    Global::videoendtime = ui->EndTime->text();

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

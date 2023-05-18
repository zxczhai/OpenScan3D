#include "dialog_sfm.h"
#include "ui_dialog_sfm.h"
#include "message.hpp"
Dialog_SFM::Dialog_SFM(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_SFM)
{
    ui->setupUi(this);
}

Dialog_SFM::~Dialog_SFM()
{
    delete ui;
}

void Dialog_SFM::on_pushButton_browseInputDir_clicked()
{
    Global::matchesDir = QFileDialog::getExistingDirectory(this, u8"浏览图片文件夹 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::matchesDir);
}

void Dialog_SFM::on_pushButton_browseOutputDir_clicked()
{
    Global::sfmOutputDir = QFileDialog::getExistingDirectory(this, u8"浏览图片文件夹 ", "", NULL);
    ui->lineEdit_OutputDir->setText(Global::sfmOutputDir);
}

void Dialog_SFM::on_btn_CONFIRM_clicked()
{
    QString matchesDir, sfmOutputDir, sfmEngine, isRobustTriangulation;

    if (ui->lineEdit_inputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入匹配数据路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    if (ui->lineEdit_OutputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入结果输出路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    matchesDir = ui->lineEdit_inputDir->text();
    Global::matchesDir = matchesDir;

    sfmOutputDir = ui->lineEdit_OutputDir->text();
    Global::sfmOutputDir = sfmOutputDir;

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
        QString head = "sfm&sfp\n";
        cmdcache.write(head.toUtf8());
        cmdcache.write(matchesDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(sfmOutputDir.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(sfmEngine.toUtf8());
        cmdcache.write("\n");
        cmdcache.write(isRobustTriangulation.toUtf8());
        cmdcache.write("\n");

        cmdcache.close();
        QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Ok);
        congmsgbuf msg;
        msg.mtype = 1;
        msg.data[0] = CMD_SFMANDSFP;
        sendMessage(msg);
        this->close();
    }
    else
    {
        QMessageBox::information(this, u8"错误", u8"无法写入缓存，请检查权限，或使用管理员身份运行 ", QMessageBox::Ok);
    }

}

void Dialog_SFM::on_btn_CANCEL_clicked()
{
    this->close();
}

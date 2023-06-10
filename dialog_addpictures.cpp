#include "dialog_addpictures.h"
#include "ui_dialog_addpictures.h"
#include <QtDebug>

Dialog_AddPictures::Dialog_AddPictures(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_AddPictures)
{
    ui->setupUi(this);
}

Dialog_AddPictures::~Dialog_AddPictures()
{
    delete ui;
}

void Dialog_AddPictures::on_pushButton_browseInputDir_clicked()
{
    Global::imagesInputDir = QFileDialog::getExistingDirectory(this, u8"浏览图片文件夹 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::imagesInputDir);
}

void Dialog_AddPictures::on_pushButton_browseDatabaseDir_clicked()
{
    Global::sensorWidthDatabaseDir = QFileDialog::getOpenFileName(this, tr("选择相机数据库文件"), QDir::homePath(), tr("All files(*.*)"));
    ui->lineEdit_databaseDir->setText(Global::sensorWidthDatabaseDir);
}

void Dialog_AddPictures::on_btn_CONFIRM_clicked()
{
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

    if (ui->lineEdit_eigenMatrix->text() == "")
    {
        if (QMessageBox::warning(this, u8"未输入本征矩阵 ", u8"忽略本征矩阵参数 ", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes) != QMessageBox::Yes)
        {
            return;
        }
        else
            Global::eigenMatrix = "NULL";
    }
    else
        Global::eigenMatrix = ui->lineEdit_eigenMatrix->text();

    Global::imagesInputDir = ui->lineEdit_inputDir->text();

    Global::sensorWidthDatabaseDir = ui->lineEdit_databaseDir->text();

    //获取图片文件夹名称
    QDir path(ui->lineEdit_inputDir->text());
    Global::imagesGroup = path.dirName();

    //计算文件夹中的图片数量
    QString folderPath = ui->lineEdit_inputDir->text();
    QDir folderDir(folderPath);
    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";
    folderDir.setNameFilters(filters);

    QFileInfoList fileList = folderDir.entryInfoList();
    int imageCount = fileList.count();
    Global::numberOfImages = imageCount;

    //获取图片的传感器尺寸和焦距






    this->close();
}

void Dialog_AddPictures::on_btn_CANCEL_clicked()
{
    this->close();
}

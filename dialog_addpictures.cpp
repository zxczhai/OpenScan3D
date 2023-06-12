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

    //获取文件夹路径
    QString folder_path =ui->lineEdit_inputDir->text();

    // 检查文件夹路径是否正确
    if (!QDir(folder_path).exists()) {
        QMessageBox::warning(this, "错误", "所选路径不是有效的文件夹路径，请重新选择！");
        ui->lineEdit_inputDir->clear();
    }
    else {
        // 检查文件夹是否为空
        if (QDir(folder_path).isEmpty()) {
            QMessageBox::information(this, "提示", "所选文件夹中没有包含任何文件，请重新选择！");
            ui->lineEdit_inputDir->clear();
        }
        else {
            // 检查是否存在与指定格式匹配的图像文件
            QStringList image_formats = {"jpg", "jpeg", "png", "bmp"};
            bool format_matching = false;
            QDirIterator it(folder_path, QStringList(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
            while (it.hasNext()) {
                it.next();
                if (image_formats.contains(QFileInfo(it.filePath()).suffix().toLower())) {
                    format_matching = true;
                    break;
                }
            }
            if (!format_matching) {
                QMessageBox::warning(this, "错误", "所选文件夹中不包含任何与指定格式匹配的图像文件，请重新选择！");
                ui->lineEdit_inputDir->clear();
            }
            else {
                // 检查是否存在其他类型的文件（除了图像文件）
                bool contains_other_files = false;
                QDirIterator it(folder_path, QStringList(), QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
                while (it.hasNext()) {
                    it.next();
                    if (!image_formats.contains(QFileInfo(it.filePath()).suffix().toLower())) {
                        contains_other_files = true;
                        break;
                    }
                }
                if (contains_other_files) {
                    QMessageBox::warning(this, "错误", "所选文件夹中包含其他类型的文件，请重新选择！");
                    ui->lineEdit_inputDir->clear();
                }
                else {
                    // 检查图像文件格式是否一致
                    QStringList image_formats = {"jpg", "jpeg", "png", "bmp"};
                    QDirIterator it(folder_path, QStringList(), QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);
                    QStringList format_list;
                    while (it.hasNext()) {
                        it.next();
                        if (image_formats.contains(QFileInfo(it.filePath()).suffix().toLower())) {
                            QString suffix = QFileInfo(it.filePath()).suffix().toLower();
                            if (!format_list.contains(suffix)) {
                                format_list.append(suffix);
                            }
                        }
                    }
                    if (format_list.size() > 1) {
                        QMessageBox::warning(this, "错误", "所选文件夹中包含格式不一致的图像文件，请重新选择！");
                        ui->lineEdit_inputDir->clear();
                    }
                    else {
                        // 限制图像数量
                        int min_num_images = 10;
                        int max_num_images = 500;
                        int num_images = QDir(folder_path).count()-2;  // 减去.和..两个特殊目录
                        if (num_images < min_num_images || num_images > max_num_images) {
                            QMessageBox::warning(this, "错误", QString("请确保所选文件夹中包含 %1 到 %2 张图像文件！").arg(min_num_images).arg(max_num_images));
                            ui->lineEdit_inputDir->clear();
                        }
                    }
                }
            }
        }
    }
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

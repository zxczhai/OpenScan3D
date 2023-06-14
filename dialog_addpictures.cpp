#include "dialog_addpictures.h"
#include "ui_dialog_addpictures.h"
#include <QtDebug>
#include <opencv2/opencv.hpp>
#include <exiv2/exiv2.hpp>
#include <QFile>
#include <QTextStream>

Dialog_AddPictures::Dialog_AddPictures(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_AddPictures)
{
    ui->setupUi(this);
    ui->label_CM->setVisible(false);
    ui->label_SS->setVisible(false);
    ui->lineEdit_cameraModel->setVisible(false);
    ui->lineEdit_sensorSize->setVisible(false);
    ui->lineEdit_focal->setVisible(false);
    ui->label_Focal->setVisible(false);
    ui->pushButton_addCameraParameters->setVisible(false);
}

Dialog_AddPictures::~Dialog_AddPictures()
{
    delete ui;
}

// 将指定路径的文件内容读入到cameraModels列表中，并存储相机模型与传感器尺寸的对应关系
void Dialog_AddPictures::loadCameraDatabase(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(";");
        if (parts.count() == 2)
        {
            QString cameraModel = parts[0].trimmed().toLower();
            QString sensorSizeStr = parts[1].trimmed();

            bool ok;
            double sensorSize = sensorSizeStr.toDouble(&ok);
            if (ok)
                Global::cameraModelsWithSize[cameraModel] = sensorSize;

            Global::cameraModels << cameraModel;
        }
    }

    file.close();
}

void Dialog_AddPictures::on_pushButton_browseInputDir_clicked()
{
    Global::imagesInputDir = QFileDialog::getExistingDirectory(this, u8"浏览图片文件夹 ", "", NULL);
    ui->lineEdit_inputDir->setText(Global::imagesInputDir);

    //获取文件夹路径
    QString folder_path =ui->lineEdit_inputDir->text();

    if (Global::imagesInputDir.isEmpty()) {
        // 用户取消了文件夹选择，可以不执行后续的检查逻辑
        return;
    }

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

    // 获取文件夹路径
    QString folderPath1 = ui->lineEdit_inputDir->text();

    // 获取文件夹中的所有图片
    QDir folder(folderPath1);
    QStringList images = folder.entryList(QStringList() << "*.jpg" << "*.jpeg" << "*.JPG" << "*.JPEG", QDir::Files);

    // 如果没有找到任何图片，则直接返回
    if (images.isEmpty())
    {
        return;
    }

    // 获取第一张图片的路径
    QString imagePath = folderPath + "/" + images.first();

    // 加载图片元数据
    std::unique_ptr<Exiv2::Image> image = Exiv2::ImageFactory::open(imagePath.toStdString());
    image->readMetadata();

    // 获取相机型号
    Exiv2::ExifData& exifData = image->exifData();
    std::string cameraModel = exifData["Exif.Image.Model"].toString();

    // 将结果输出到控制台
    qDebug() << "Camera model: " << QString::fromStdString(cameraModel);
    Global::cameraModel = QString::fromStdString(cameraModel);

    QString sensorWidthDatabasePath = QCoreApplication::applicationDirPath() + "/sensor_width_camera_database.txt";
    Global::sensorWidthDatabaseDir = sensorWidthDatabasePath;
    qDebug() << sensorWidthDatabasePath;

    loadCameraDatabase(sensorWidthDatabasePath);

    QString cameraModel1 = Global::cameraModel.trimmed().toLower();

    if (cameraModel1.isEmpty())
    {
        ui->tips->setText("该文件夹图片缺少相机参数，请添加！");
        ui->label_CM->setVisible(true);
        ui->label_SS->setVisible(true);
        ui->lineEdit_cameraModel->setVisible(true);
        ui->lineEdit_sensorSize->setVisible(true);
        ui->pushButton_addCameraParameters->setVisible(true);
        ui->lineEdit_focal->setVisible(true);
        ui->label_Focal->setVisible(true);
        Global::sensorSize = "vacant";
        Global::cameraModel = "vacant";
        Global::signPictures =true;
    }
    else if (Global::cameraModelsWithSize.contains(cameraModel1))
    {
        double sensorSize = Global::cameraModelsWithSize[cameraModel1];
        qDebug() << "Sensor size: " << sensorSize;
        Global::sensorSize.setNum(sensorSize);
        ui->tips->setText("该文件夹图片相机参数在数据库内！");
        ui->label_CM->setVisible(false);
        ui->label_SS->setVisible(false);
        ui->lineEdit_cameraModel->setVisible(false);
        ui->lineEdit_sensorSize->setVisible(false);
        ui->lineEdit_focal->setVisible(false);
        ui->label_Focal->setVisible(false);
        Global::isFilled =true;
    }
    else
    {
        ui->tips->setText("该文件夹图片相机参数并不在数据库内，请添加！");
        ui->label_CM->setVisible(true);
        ui->label_SS->setVisible(true);
        ui->lineEdit_cameraModel->setVisible(true);
        ui->lineEdit_sensorSize->setVisible(true);
        ui->pushButton_addCameraParameters->setVisible(true);
        ui->lineEdit_focal->setVisible(true);
        ui->label_Focal->setVisible(true);
        ui->lineEdit_cameraModel->setText(cameraModel1);
        Global::sensorSize = "vacant";
        Global::cameraModel = "vacant";
        Global::signPictures =true;
    }

}

void Dialog_AddPictures::on_pushButton_addCameraParameters_clicked()
{

    if(ui->lineEdit_sensorSize->text().isEmpty() || ui->lineEdit_cameraModel->text().isEmpty() || ui->lineEdit_focal->text().isEmpty())
    {
        QMessageBox::critical(this, u8"错误 ", u8"填写不完整，添加失败！ ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else if(!QRegExp("^-?[0-9]+([.][0-9]*)?$").exactMatch(ui->lineEdit_sensorSize->text()) || !QRegExp("^-?[0-9]+([.][0-9]*)?$").exactMatch(ui->lineEdit_focal->text()))
    {
        QMessageBox::critical(this, u8"错误 ", u8"传感器尺寸和焦距必须为数字，添加失败！ ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    else
    {
        Global::sensorSize = ui->lineEdit_sensorSize->text();
        Global::cameraModel = ui->lineEdit_cameraModel->text();
        Global::focal = ui->lineEdit_focal->text();

        QFile file(Global::sensorWidthDatabaseDir);
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            // 将 Global::cameraModel 和 Global::sensorSize 变量写入文件
            out << Global::cameraModel << ";" << Global::sensorSize << endl;
            file.close();
        }

        Global::isFilled = true;
        QMessageBox::information(this, u8"完成", u8"添加成功！", QMessageBox::Yes);
        return;
    }

}


//void Dialog_AddPictures::on_pushButton_browseDatabaseDir_clicked()
//{
//    Global::sensorWidthDatabaseDir = QFileDialog::getOpenFileName(this, tr("选择相机数据库文件"), QDir::homePath(), tr("All files(*.*)"));
//    ui->lineEdit_databaseDir->setText(Global::sensorWidthDatabaseDir);
//}

void Dialog_AddPictures::on_btn_CONFIRM_clicked()
{
    if (ui->lineEdit_inputDir->text() == "")
    {
        QMessageBox::critical(this, u8"错误 ", u8"未输入图片路径 ", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    //    if (ui->lineEdit_databaseDir->text() == "")
    //    {
    //        QMessageBox::critical(this, u8"错误 ", u8"未选择相机数据库文件 ", QMessageBox::Ok, QMessageBox::Ok);
    //        return;
    //    }
    if(!Global::isFilled)
    {
        QMessageBox::critical(this, u8"错误 ", u8"请先添加相机参数！！ ", QMessageBox::Ok, QMessageBox::Ok);
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


    if(Global::signPictures)
    {
        /*将参数写入文件*/
        // 检查是否存在tmp文件夹
        if (!QDir("/tmp").exists())
        {
            // 如果不存在就创建tmp目录
            QDir().mkpath("/tmp");
        }
        // 创建OpenScan3D目录
        QDir("/tmp").mkdir(".OpenScan3D");

        QFile cameraCache("/tmp/.OpenScan3D/cameraCache.tmp");

        if (cameraCache.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        {
            cameraCache.write(Global::cameraModel.toUtf8());
            cameraCache.write("\n");
            cameraCache.write(Global::sensorSize.toUtf8());
            cameraCache.write("\n");
            cameraCache.write(Global::focal.toUtf8());
            cameraCache.write("\n");
            cameraCache.close();
            QMessageBox::information(this, u8"完成", u8"配置完成 ", QMessageBox::Yes);
        }
        else
        {
            QMessageBox::information(this, u8"错误", u8"无法访问缓存文件，请检查权限，或使用管理员身份运行 ", QMessageBox::Yes);
        }
    }


    ui->lineEdit_sensorSize->clear();
    ui->lineEdit_cameraModel->clear();
    ui->lineEdit_eigenMatrix->clear();
    ui->lineEdit_inputDir->clear();
    ui->lineEdit_focal->clear();
    ui->label_CM->setVisible(false);
    ui->label_SS->setVisible(false);
    ui->lineEdit_cameraModel->setVisible(false);
    ui->lineEdit_sensorSize->setVisible(false);
    ui->lineEdit_focal->setVisible(false);
    ui->label_Focal->setVisible(false);
    Global::isFilled =false;
    Global::signPictures =false;

    this->close();
}

void Dialog_AddPictures::on_btn_CANCEL_clicked()
{
    this->close();
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include "viewer.h"
#include "embedExternalApp.h"
#include <QDebug>
#include <QPropertyAnimation>
#include <QStandardItemModel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::timerSlot);
    timer->start(500);
    ui->pushButton->setText("new button");

    //设置分步三维重建进度显示初始颜色
    ui->commandLinkButton->setStyleSheet("color: #FFB300;");
    ui->commandLinkButton_2->setStyleSheet("color: #FFB300;");
    ui->commandLinkButton_3->setStyleSheet("color: #FFB300;");
    ui->commandLinkButton_4->setStyleSheet("color: #FFB300;");
    ui->commandLinkButton_5->setStyleSheet("color: #FFB300;");

    //    //设置图片集显示列表
    //    QStandardItemModel *model=new QStandardItemModel () ;
    //    ui->tableView->setModel(model);
    //    //设置列表行头
    //    int width = 150;  // 标准项的宽度（像素）
    //    model->setHorizontalHeaderItem(0,new QStandardItem("Images Group"));
    //    ui->tableView->setColumnWidth(0, width);
    //    model->setHorizontalHeaderItem(1, new QStandardItem ("Number Of Images"));
    //    ui->tableView->setColumnWidth(1, width);
    //    model->setHorizontalHeaderItem(2, new QStandardItem ("Sensor Size"));
    //    ui->tableView->setColumnWidth(2, width);
    //    model->setHorizontalHeaderItem(3, new QStandardItem ("Focus"));
    //    ui->tableView->setColumnWidth(3, width);
    //    //设置列表列头
    //    model->setVerticalHeaderItem(0, new QStandardItem ("-")) ;
    //    //数据：
    //    QStandardItem* item0 = new QStandardItem(Global::imagesGroup);  // 创建标准项
    //    item0->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    //    model->setItem(0, 0, item0);  // 设置标准项到指定位置
    //    QStandardItem* item1 = new QStandardItem(Global::numberOfImages);  // 创建标准项
    //    item1->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    //    model->setItem(0, 1, item1);  // 设置标准项到指定位置
    //    QStandardItem* item2 = new QStandardItem(Global::sensorSize);  // 创建标准项
    //    item2->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    //    model->setItem(0, 2, item2);  // 设置标准项到指定位置
    //    QStandardItem* item3 = new QStandardItem(Global::focus);  // 创建标准项
    //    item3->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    //    model->setItem(0, 3, item3);  // 设置标准项到指定位置

}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
}

Window linuxFindWin(std::string winName)
{
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        std::cout<<"conneect failed\n";
        // 连接失败
    }
    Window root = DefaultRootWindow(display);
    Window found = None;

    unsigned int numWindows;
    Window *windows;
    XQueryTree(display, root, &root, &root, &windows, &numWindows);

    for (unsigned int i = 0; i < numWindows; ++i) {
        XClassHint classHint;
        if (XGetClassHint(display, windows[i], &classHint)) {
            if (strcmp(classHint.res_class, winName.c_str()) == 0) { // 根据窗口类别判断是否为计算器
                found = windows[i];
                break;
            }
        }
    }

    XFree(windows);

    if (found == None) {
        // 没有找到窗口
        std::cout<<"failed to find window\n";
        return 0;
    }
    return found;
}

void MainWindow::on_pushButton_clicked()
{
    //    choose_file *newFileChoose = new choose_file(this);
    //    newFileChoose->show();

    //    std::string winName = "qtcreator";
    //    ui->widget = new viewer(linuxFindWin(winName));
    //    ui->widget->setObjectName(QString::fromUtf8("widget"));
    //    ui->widget->setGeometry(QRect(10, 70, 1361, 661));
    //    ui->widget->show();
    //    ui->widget->update();

    embedexternalapp embed = new embedexternalapp(this);
    //    delete this->ui->widget;
    //    ui->widget = &embed;
    //    ui->widget->setObjectName(QString::fromUtf8("widget"));
    //    ui->widget->setGeometry(QRect(50, 40, 841, 481));
    //    ui->widget->show();
    //    ui->widget->update();
}


void MainWindow::on_actionOpen_O_triggered()
{
    //    choose_file *newChooseFile = new choose_file(this);
    //    newChooseFile->show();
    QString fileName = QFileDialog::getOpenFileName(
                this,
                tr("open a 3D file"),
                tr("/root/"),
                tr("All files(*.*)")
                );
    if (fileName.isEmpty()) {
        QMessageBox::warning(this, "Warning!", "Failed to open the model!");
    }
    else {
        std::cout<<"Success";
    }
}


void MainWindow::on_actionFeatureMatch_triggered()
{
    dlgfm.setWindowTitle("特征匹配配置"); // 设置窗口标题
    dlgfm.move(geometry().center() - dlgfm.rect().center()); // 将对话框移动到MainWindow的中心
    dlgfm.exec();
}

void MainWindow::on_actionSFMAndSparseCloudGen_triggered()
{
    dlgsfm.setWindowTitle("三维重建配置"); // 设置窗口标题
    int x = geometry().center().x() - dlgsfm.width() / 2;
    int y = geometry().center().y() - dlgsfm.height() * 2 / 3; // 将对话框移动到MainWindow的正中间三分之一处
    dlgsfm.move(x, y); // 将对话框移动到指定位置
    dlgsfm.exec();
}

void MainWindow::on_actionDenseCloudGen_triggered()
{
    dlgdense.setWindowTitle("密集点云生成"); // 设置窗口标题
    int x = geometry().center().x() - dlgdense.width() / 2;
    int y = geometry().center().y() - dlgdense.height() * 2 / 3; // 将对话框移动到MainWindow的正中间三分之一处
    dlgdense.move(x, y); // 将对话框移动到指定位置
    dlgdense.exec();
}

void MainWindow::on_actionTriMeshRecon_triggered()
{
    dlgtmr.setWindowTitle("三角网模型重建"); // 设置窗口标题
    int x = geometry().center().x() - dlgtmr.width() / 2;
    int y = geometry().center().y() - dlgtmr.height() * 2 / 3; // 将对话框移动到MainWindow的正中间三分之一处
    dlgtmr.move(x, y); // 将对话框移动到指定位置
    dlgtmr.exec();
}

void MainWindow::on_actionTextureMesh_triggered()
{
    dlgtm.setWindowTitle("纹理映射"); // 设置窗口标题
    int x = geometry().center().x() - dlgtm.width() / 2;
    int y = geometry().center().y() - dlgtm.height() * 2 / 3; // 将对话框移动到MainWindow的正中间三分之一处
    dlgtm.move(x, y); // 将对话框移动到指定位置
    dlgtm.exec();
}

void MainWindow::on_actionAuto_Properties_triggered()
{
    dlgar.setWindowTitle("一键自动重建"); // 设置窗口标题
    dlgar.move(geometry().center() - dlgar.rect().center()); // 将对话框移动到MainWindow的中心
    dlgar.exec();
}

void MainWindow::on_btn_addPictures_clicked()
{
    dlgap.setWindowTitle("导入图片集"); // 设置窗口标题
    dlgap.move(geometry().center() - dlgap.rect().center()); // 将对话框移动到MainWindow的中心
    dlgap.exec();
}

void MainWindow::on_btn_importVideoFrames_clicked()
{
    dlgivf.setWindowTitle("导入视频帧"); // 设置窗口标题
    dlgivf.move(geometry().center() - dlgivf.rect().center()); // 将对话框移动到MainWindow的中心
    dlgivf.exec();
}

void MainWindow::timerSlot()
{
    //设置图片集显示列表
    QStandardItemModel *model=new QStandardItemModel () ;
    ui->tableView->setModel(model);
    //设置列表行头
    int width = 190;  // 标准项的宽度（像素）
    model->setHorizontalHeaderItem(0,new QStandardItem("Images Group"));
    ui->tableView->setColumnWidth(0, width);
    model->setHorizontalHeaderItem(1, new QStandardItem ("Number Of Images"));
    ui->tableView->setColumnWidth(1, width);
    model->setHorizontalHeaderItem(2, new QStandardItem ("cameraModel"));
    ui->tableView->setColumnWidth(2, width);
    model->setHorizontalHeaderItem(3, new QStandardItem ("Sensor Size"));
    ui->tableView->setColumnWidth(3, width);
    //设置列表列头
    model->setVerticalHeaderItem(0, new QStandardItem ("-")) ;
    //数据：
    QStandardItem* item0 = new QStandardItem(Global::imagesGroup);  // 创建标准项
    item0->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    model->setItem(0, 0, item0);  // 设置标准项到指定位置
    QStandardItem* item1 = new QStandardItem(QString::number(Global::numberOfImages));  // 创建标准项
    item1->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    model->setItem(0, 1, item1);  // 设置标准项到指定位置
    QStandardItem* item2 = new QStandardItem(Global::cameraModel);  // 创建标准项
    item2->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    model->setItem(0, 2, item2);  // 设置标准项到指定位置
    QStandardItem* item3 = new QStandardItem(Global::sensorSize);  // 创建标准项
    item3->setData(Qt::AlignCenter, Qt::TextAlignmentRole);  // 设置文本居中对齐
    model->setItem(0, 3, item3);  // 设置标准项到指定位置

    if (Global::GetProcessIdFromName("R3D") != 0)
    {
        Global::connectEngine();
        if (ui->label_engine->text() != u8"R3D is connected! ")
        {
            QPalette pa;
            pa.setColor(QPalette::WindowText, Qt::green);
            ui->label_engine->setPalette(pa);
            ui->label_engine->setText(u8"R3D is connected!");
        }
    }
    else
    {
        if (ui->label_engine->text() != u8"R3D is not connected!")
        {
            QPalette pa;
            pa.setColor(QPalette::WindowText, Qt::red);
            ui->label_engine->setPalette(pa);
            ui->label_engine->setText(u8"R3D is not connected!");
        }
    }

    if (Global::tasking)
    {
        QFile file("/tmp/.OpenScan3D/cmdCache.tmp");  // 打开文件
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;  // 若打开文件失败则返回

        QTextStream in(&file);  // 创建文本流对象
        QString line = in.readLine();  // 读取第一行内容
        file.close();  // 关闭文件

        ui->progressBar->setMinimum(0);  // 设置进度条最小值
        ui->progressBar->setMaximum(100);  // 设置进度条最大值
        int value = 0;
        if (line == "matchfeature") {
            value=20;
            ui->commandLinkButton->setStyleSheet("color: green;");
        } else if (line == "sfm&sfp") {
            value=40;
            ui->commandLinkButton_2->setStyleSheet("color: green;");
        } else if (line == "DENSIFYPOINTCLOUD") {
            value=60;
            ui->commandLinkButton_3->setStyleSheet("color: green;");
        } else if (line == "RECONSTRUCTMESH"){
            value=80;
            ui->commandLinkButton_4->setStyleSheet("color: green;");
        } else if (line == "TEXTUREMESH"){
            value=100;
            ui->commandLinkButton_5->setStyleSheet("color: green;");
        }

        QPropertyAnimation *animation = new QPropertyAnimation(ui->progressBar, "value");  // 访问UI界面中的进度条控件
        animation->setDuration(500);  // 动画持续时间为500毫秒
        animation->setStartValue(ui->progressBar->value());
        animation->setEndValue(value);
        animation->start(QAbstractAnimation::DeleteWhenStopped);

        // 将进度条添加到窗口中显示
        ui->progressBar->show();  // 访问UI界面中的进度条控件
    }

    if(Global::autoTasking)
    {

    }
}

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
#include "viewer.h"
#include "embedExternalApp.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButton->setText("new button");

}

MainWindow::~MainWindow()
{
    delete ui;
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

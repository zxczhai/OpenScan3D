#include "choose_file.h"
#include "ui_choose_file.h"
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>
#include <QString>
choose_file::choose_file(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::choose_file)
{
    ui->setupUi(this);
}

choose_file::~choose_file()
{
    delete ui;
}

void choose_file::on_pushButton_clicked()
{
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


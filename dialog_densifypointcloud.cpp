#include "dialog_densifypointcloud.h"
#include "ui_dialog_densifypointcloud.h"

Dialog_DensifyPointCloud::Dialog_DensifyPointCloud(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_DensifyPointCloud)
{
    ui->setupUi(this);
}

Dialog_DensifyPointCloud::~Dialog_DensifyPointCloud()
{
    delete ui;
}

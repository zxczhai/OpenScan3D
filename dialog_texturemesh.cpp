#include "dialog_texturemesh.h"
#include "ui_dialog_texturemesh.h"

Dialog_TextureMesh::Dialog_TextureMesh(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_TextureMesh)
{
    ui->setupUi(this);
}

Dialog_TextureMesh::~Dialog_TextureMesh()
{
    delete ui;
}

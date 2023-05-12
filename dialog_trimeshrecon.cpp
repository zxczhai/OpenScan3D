#include "dialog_trimeshrecon.h"
#include "ui_dialog_trimeshrecon.h"

Dialog_TriMeshRecon::Dialog_TriMeshRecon(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_TriMeshRecon)
{
    ui->setupUi(this);
}

Dialog_TriMeshRecon::~Dialog_TriMeshRecon()
{
    delete ui;
}

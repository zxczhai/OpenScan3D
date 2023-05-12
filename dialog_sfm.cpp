#include "dialog_sfm.h"
#include "ui_dialog_sfm.h"

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

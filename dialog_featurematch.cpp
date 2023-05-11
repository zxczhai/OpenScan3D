#include "dialog_featurematch.h"
#include "ui_dialog_featurematch.h"

Dialog_FeatureMatch::Dialog_FeatureMatch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog_FeatureMatch)
{
    ui->setupUi(this);
}

Dialog_FeatureMatch::~Dialog_FeatureMatch()
{
    delete ui;
}

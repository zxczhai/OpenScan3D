#ifndef DIALOG_FEATUREMATCH_H
#define DIALOG_FEATUREMATCH_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QDir>
#include "global.h"
namespace Ui {
class Dialog_FeatureMatch;
}

class Dialog_FeatureMatch : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_FeatureMatch(QWidget *parent = nullptr);
    ~Dialog_FeatureMatch();

private slots:

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();



private:
    Ui::Dialog_FeatureMatch *ui;
};

#endif // DIALOG_FEATUREMATCH_H

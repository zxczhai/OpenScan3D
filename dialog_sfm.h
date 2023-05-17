#ifndef DIALOG_SFM_H
#define DIALOG_SFM_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include "global.h"

namespace Ui {
class Dialog_SFM;
}

class Dialog_SFM : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SFM(QWidget *parent = nullptr);
    ~Dialog_SFM();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_SFM *ui;
};

#endif // DIALOG_SFM_H

#ifndef DIALOG_AUTOREBUILD_H
#define DIALOG_AUTOREBUILD_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include "global.h"

namespace Ui {
class Dialog_AutoRebuild;
}

class Dialog_AutoRebuild : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AutoRebuild(QWidget *parent = nullptr);
    ~Dialog_AutoRebuild();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseDatabaseDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_AutoRebuild *ui;
};

#endif // DIALOG_AUTOREBUILD_H

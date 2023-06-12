#ifndef DIALOG_ADDPICTURES_H
#define DIALOG_ADDPICTURES_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QStringList>
#include <QDirIterator>
#include <QString>
#include <QDir>
#include "global.h"

namespace Ui {
class Dialog_AddPictures;
}

class Dialog_AddPictures : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_AddPictures(QWidget *parent = nullptr);
    ~Dialog_AddPictures();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseDatabaseDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_AddPictures *ui;
};

#endif // DIALOG_ADDPICTURES_H

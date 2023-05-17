#ifndef DIALOG_TRIMESHRECON_H
#define DIALOG_TRIMESHRECON_H

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include "global.h"

namespace Ui {
class Dialog_TriMeshRecon;
}

class Dialog_TriMeshRecon : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_TriMeshRecon(QWidget *parent = nullptr);
    ~Dialog_TriMeshRecon();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();

private:
    Ui::Dialog_TriMeshRecon *ui;
};

#endif // DIALOG_TRIMESHRECON_H

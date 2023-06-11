#ifndef DIALOG_IMPORTVIDEOFRAMES_H
#define DIALOG_IMPORTVIDEOFRAMES_H

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QMessageBox>
#include <QDir>
#include <QTimer>
#include <QDebug>
#include <QProgressDialog>
#include "global.h"

namespace Ui {
class Dialog_ImportVideoFrames;
}

class Dialog_ImportVideoFrames : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_ImportVideoFrames(QWidget *parent = nullptr);
    ~Dialog_ImportVideoFrames();

private slots:
    void on_pushButton_browseInputDir_clicked();

    void on_pushButton_browseOutputDir_clicked();

    void on_btn_CONFIRM_clicked();

    void on_btn_CANCEL_clicked();

//    bool isDirectoryEmpty(QString path);

    void extract_keyframes(const string& video_file, const string& output_dir);


private:
    Ui::Dialog_ImportVideoFrames *ui;

};

#endif // DIALOG_IMPORTVIDEOFRAMES_H

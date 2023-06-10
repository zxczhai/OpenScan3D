#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog_featurematch.h"
#include "dialog_sfm.h"
#include "dialog_densifypointcloud.h"
#include "dialog_trimeshrecon.h"
#include "dialog_texturemesh.h"
#include "dialog_autorebuild.h"
#include "dialog_importvideoframes.h"
#include "dialog_addpictures.h"
#include "global.h"
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_actionOpen_O_triggered();

    void on_actionFeatureMatch_triggered();

    void on_actionSFMAndSparseCloudGen_triggered();

    void on_actionDenseCloudGen_triggered();

    void on_actionTriMeshRecon_triggered();

    void on_actionTextureMesh_triggered();

    void timerSlot();

    void on_actionAuto_Properties_triggered();

    void on_btn_addPictures_clicked();

    void on_btn_importVideoFrames_clicked();

private:
    Ui::MainWindow *ui;
    Dialog_FeatureMatch dlgfm;
    Dialog_SFM dlgsfm;
    Dialog_DensifyPointCloud dlgdense;
    Dialog_TriMeshRecon dlgtmr;
    Dialog_TextureMesh dlgtm;
    Dialog_AutoRebuild dlgar;
    Dialog_ImportVideoFrames dlgivf;
    Dialog_AddPictures dlgap;
    QTimer* timer;
};
#endif // MAINWINDOW_H

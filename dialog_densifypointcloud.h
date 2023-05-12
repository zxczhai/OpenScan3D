#ifndef DIALOG_DENSIFYPOINTCLOUD_H
#define DIALOG_DENSIFYPOINTCLOUD_H

#include <QDialog>

namespace Ui {
class Dialog_DensifyPointCloud;
}

class Dialog_DensifyPointCloud : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_DensifyPointCloud(QWidget *parent = nullptr);
    ~Dialog_DensifyPointCloud();

private:
    Ui::Dialog_DensifyPointCloud *ui;
};

#endif // DIALOG_DENSIFYPOINTCLOUD_H

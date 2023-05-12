#ifndef DIALOG_TRIMESHRECON_H
#define DIALOG_TRIMESHRECON_H

#include <QDialog>

namespace Ui {
class Dialog_TriMeshRecon;
}

class Dialog_TriMeshRecon : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_TriMeshRecon(QWidget *parent = nullptr);
    ~Dialog_TriMeshRecon();

private:
    Ui::Dialog_TriMeshRecon *ui;
};

#endif // DIALOG_TRIMESHRECON_H

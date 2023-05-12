#ifndef DIALOG_TEXTUREMESH_H
#define DIALOG_TEXTUREMESH_H

#include <QDialog>

namespace Ui {
class Dialog_TextureMesh;
}

class Dialog_TextureMesh : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_TextureMesh(QWidget *parent = nullptr);
    ~Dialog_TextureMesh();

private:
    Ui::Dialog_TextureMesh *ui;
};

#endif // DIALOG_TEXTUREMESH_H

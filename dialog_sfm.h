#ifndef DIALOG_SFM_H
#define DIALOG_SFM_H

#include <QDialog>

namespace Ui {
class Dialog_SFM;
}

class Dialog_SFM : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_SFM(QWidget *parent = nullptr);
    ~Dialog_SFM();

private:
    Ui::Dialog_SFM *ui;
};

#endif // DIALOG_SFM_H

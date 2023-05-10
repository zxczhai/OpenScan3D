#ifndef LOGINDLG_H
#define LOGINDLG_H

#include <QDialog>

namespace Ui {
class LoginDlg;
}

class LoginDlg : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDlg(QWidget *parent = nullptr);
    ~LoginDlg();

private slots:
    void on_pushButton_clicked();

private:
    Ui::LoginDlg *ui;
};

#endif // LOGINDLG_H

#ifndef CHOOSE_FILE_H
#define CHOOSE_FILE_H

#include <QDialog>

namespace Ui {
class choose_file;
}

class choose_file : public QDialog
{
    Q_OBJECT

public:
    explicit choose_file(QWidget *parent = nullptr);
    ~choose_file();

private slots:
    void on_pushButton_clicked();

private:
    Ui::choose_file *ui;
};

#endif // CHOOSE_FILE_H

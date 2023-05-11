#ifndef DIALOG_FEATUREMATCH_H
#define DIALOG_FEATUREMATCH_H

#include <QDialog>

namespace Ui {
class Dialog_FeatureMatch;
}

class Dialog_FeatureMatch : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog_FeatureMatch(QWidget *parent = nullptr);
    ~Dialog_FeatureMatch();

private:
    Ui::Dialog_FeatureMatch *ui;
};

#endif // DIALOG_FEATUREMATCH_H

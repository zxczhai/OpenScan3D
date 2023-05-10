#ifndef QMYWIDGET_H
#define QMYWIDGET_H


#pragma once

#include <QWidget>
#include <QWindow>
class QMyWidget : public QWidget {
public:
    QMyWidget(QWindow *window) : QWidget() {
        create(window->winId());
    }
};

#endif // QMYWIDGET_H

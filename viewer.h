#ifndef VIEWER_H
#define VIEWER_H

#include <QObject>
#include <QWidget>
#include <QWindow>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <string.h>

class viewer : public QWidget
{
    Q_OBJECT
public:
    explicit viewer(QWidget *parent = nullptr);
    explicit viewer(Window found,QWidget *parent = nullptr);
//    static Window linuxFindWin(std::string winName);

signals:

};

#endif // VIEWER_H

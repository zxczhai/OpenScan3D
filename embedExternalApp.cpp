#include "embedExternalApp.h"
#include <QProcess>
#include <QThread>
#include <QWindow>
#include <QVBoxLayout>
#include <QScrollArea>
#include "utils.h"
#include <stdio.h>
#include <iostream>
embedexternalapp::embedexternalapp(QWidget *parent)
    : QWidget(parent)
{
    QList<qint64> pidlist;
    QStringList programs;
    programs
             << "/usr/local/bin/OpenMVS/Viewer";
    foreach (QString pro, programs) {
        QProcess process;
        qint64 pid;
        if (process.startDetached(pro, QStringList(), "", &pid)) {
            pidlist.push_back(pid);
        }
    }
    QThread::currentThread()->msleep(500);
    //等待一会，虽然进程启动但如果窗口没有显示则无法通过Pid获取窗口Id
//    QVBoxLayout *mainlayout = new QVBoxLayout(this);
//    foreach (qint64 pid, pidlist) {
//        unsigned long winid = get_win_id_from_pid(pid);
//        if (0 == winid) {
//            //if no window shutdown the extra process
//            QProcess::execute("/bin/sh", QStringList() << "-c" << QString("kill -9 %1").arg(pid));
//            continue;
//        }
        unsigned long winid = get_win_id_from_pid(pidlist.at(0));
        QWindow *window = QWindow::fromWinId(winid);
        window->setFlags(window->flags()| Qt::CustomizeWindowHint | Qt::WindowTitleHint);
        QWidget *widget = QWidget::createWindowContainer(window,this);
        widget->setMinimumSize(1000, 600);
//        mainlayout->addWidget(widget);
//    }

}

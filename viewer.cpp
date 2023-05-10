#include "viewer.h"
#include "qmywidget.h"
#include <iostream>
viewer::viewer(QWidget *parent)
    : QWidget{parent}
{

}

viewer::viewer(Window found,QWidget *parent)
    : QWidget(parent)
{
    QWindow *window = QWindow::fromWinId(found);
    if (window == nullptr) {
        // 转换失败
        std::cout<<"failed\n";
    }
    window->setFlags(window->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
    QWidget *m_widget;
    m_widget = QWidget::createWindowContainer(window, this);
    m_widget->setMinimumSize(1361, 661);
    //    QMyWidget widget(window);

}

//static Window linuxFindWin(std::string winName)
//{
//    Display* display = XOpenDisplay(nullptr);
//    if (display == nullptr) {
//        // 连接失败
//    }
//    Window root = DefaultRootWindow(display);
//    Window found = None;

//    unsigned int numWindows;
//    Window *windows;
//    XQueryTree(display, root, &root, &root, &windows, &numWindows);

//    for (unsigned int i = 0; i < numWindows; ++i) {
//        XClassHint classHint;
//        if (XGetClassHint(display, windows[i], &classHint)) {
//            if (strcmp(classHint.res_class, winName.c_str()) == 0) { // 根据窗口类别判断是否为计算器
//                found = windows[i];
//                break;
//            }
//        }
//    }

//    XFree(windows);

//    if (found == None) {
//        // 没有找到窗口
//        return 0;
//    }
//    return found;
//}


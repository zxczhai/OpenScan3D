#include "utils.h"

#include <QDebug>

#include <list>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

void search(unsigned long w, void *display, unsigned long _atomPID, int _pid);

static std::list<Window> _result;
unsigned long get_win_id_from_pid(int pid)
{
    unsigned long winid = 0;
    _result.clear();
    // Get the PID property atom.
    Display *display = XOpenDisplay(0);
    Window wRoot = XDefaultRootWindow(display);
    Atom _atomPID = XInternAtom(display, "_NET_WM_PID", True);
    if (_atomPID == None) {
        return 0;
    }

    search(wRoot, display, _atomPID, pid);

    if (_result.size() > 0) {
        winid = *_result.begin();
    }
    qInfo() << _result.size();
    return winid;
}

void search(unsigned long w, void *display, unsigned long _atomPID, int _pid)
{
    Display *_display = static_cast<Display *>(display);
    // Get the PID for the current Window.
    Atom type;
    int format;
    unsigned long nItems;
    unsigned long bytesAfter;
    unsigned char *propPID = 0;
    if (Success == XGetWindowProperty(_display, w, _atomPID, 0, 1, False,
                                      XA_CARDINAL, &type, &format, &nItems,
                                      &bytesAfter, &propPID)) {
        if (propPID != 0) {
            // If the PID matches, add this window to the result set.
            if (_pid == *((unsigned long *)propPID)) {
                _result.push_back(w);
            }

            XFree(propPID);
        }
    }

    // Recurse into child windows.
    Window wRoot;
    Window wParent;
    Window *wChild;
    unsigned nChildren;
    if (0 != XQueryTree(_display, w, &wRoot, &wParent, &wChild, &nChildren)) {
        for (unsigned i = 0; i < nChildren; i++) {
            search(wChild[i], _display, _atomPID, _pid);
        }
    }

//     XFree(propPID);
}

#include "xdginhibiter.h"
#include <QApplication>
#include <QWidget>
#include <QProcess>
#include <QMessageBox>
#include <stdio.h>
#include <iostream>

#define XDG_SCREENSAVER "xdg-screensaver"
#define XDG_SUSPEND XDG_SCREENSAVER " suspend "
#define XDG_RESUME  XDG_SCREENSAVER " resume "

XDGInhibiter::XDGInhibiter()
{
}

QString XDGInhibiter::getWindowId()
{
    char strWindowId[255];
    WId windowId;
    QString ret;

    foreach (QWidget *w, QApplication::allWidgets()) {
        if (w->isWindow() && w->windowTitle() == "MainScreenPdfView") {
            windowId = w->winId();
            break;
        }
    }

    if (snprintf(strWindowId, 254, "%lu", windowId) > 0) {
        ret = QString(strWindowId);
    } else {
        std::cerr << "Error while converting WId to string." << std::endl;
        ret = QString("");
    }

    return ret;
}

bool XDGInhibiter::canHandle()
{
    QProcess::ExitStatus ret;
    QProcess xdg;
    xdg.start(XDG_SCREENSAVER " --help");
    xdg.waitForFinished();
    ret = xdg.exitStatus();
    return (ret == QProcess::NormalExit);
}

void XDGInhibiter::inhibit()
{
    QProcess xdg;
    QString suspend = QString(XDG_SUSPEND) + this->getWindowId();
    xdg.start(suspend);
    xdg.waitForFinished();
    if (xdg.exitStatus() != QProcess::NormalExit) {
        std::cerr << "Cannot suspend screensaver" << std::endl;
    }
}

void XDGInhibiter::desinhibit()
{
    QProcess xdg;
    QString resume = QString(XDG_RESUME) + this->getWindowId();
    xdg.start(resume);
    xdg.waitForFinished();
    if (xdg.exitStatus() != QProcess::NormalExit) {
        std::cerr << "Cannot resume screensaver" << std::endl;
    }
}

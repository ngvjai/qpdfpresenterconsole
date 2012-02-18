#include "screensaverinhibit.h"
#include "freedesktopinhibiter.h"
#include "xdginhibiter.h"

ScreenSaverInhibit::ScreenSaverInhibit(QObject *parent) :
    QObject(parent)
{
    this->canInhibit = false;
}

void ScreenSaverInhibit::setCanInhibit(bool v)
{
    this->canInhibit = v;
    this->handleScreenSaverInhibition();
}

bool ScreenSaverInhibit::getCanInhibit()
{
    return this->canInhibit;
}

void ScreenSaverInhibit::allowScreenSaver()
{
    this->setCanInhibit(true);
}

void ScreenSaverInhibit::dontAllowScreenSaver()
{
    this->setCanInhibit(false);
}

void ScreenSaverInhibit::handleScreenSaverInhibition()
{
    if (this->getCanInhibit()) {
        std::cerr << "Screen saver allowed, no inhibition." << std::endl;
        return;
    }

#ifdef HAVE_DBUS
    FreedesktopInhibiter fdi;
    if(fdi.canHandle()) {
        fdi.inhibit();
    } else {
        std::cerr << "Freedesktop inhibiter not valid." << std::endl;
    }
#endif

#ifdef Q_OS_UNIX || Q_OS_LINUX
    XDGInhibiter xdgi;
    if (xdgi.canHandle()) {
        xdgi.inhibit();
        return;
    } else {
        std::cerr << "XDG inhibiter not valid." << std::endl;
    }
#endif

    return;
}

#include "screensaverinhibit.h"
#include "freedesktopinhibiter.h"

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

    return;
}

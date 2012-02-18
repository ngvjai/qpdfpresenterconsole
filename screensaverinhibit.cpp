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

void ScreenSaverInhibit::setStatus(ScreenSaverStatus status)
{
    this->status = status;
}

ScreenSaverStatus ScreenSaverInhibit::getStatus()
{
    return this->status;
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
    ScreenSaverStatus current = this->getStatus();

    if (this->getCanInhibit()) {
        std::cerr << "Screen saver allowed, no inhibition." << std::endl;
        return;
    }

    if (current == SCREENSAVER_INHIBITED) {
        this->setScreenSaverDesinhibited();
    }

    if (current == SCREENSAVER_NON_INHIBITED) {
        this->setScreenSaverInhibited();
    }

    return;
}

void ScreenSaverInhibit::setScreenSaverInhibited()
{
    this->switchScreenSaverInhibition(SCREENSAVER_INHIBITED);
}

void ScreenSaverInhibit::setScreenSaverDesinhibited()
{
    this->switchScreenSaverInhibition(SCREENSAVER_NON_INHIBITED);
}

void ScreenSaverInhibit::switchScreenSaverInhibition(ScreenSaverStatus targetStatus)
{
#ifdef HAVE_DBUS
    static FreedesktopInhibiter fdi;
    if(fdi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                fdi.inhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "Freedesktop inhibiter not valid." << std::endl;
    }
#endif

#ifdef Q_OS_UNIX || Q_OS_LINUX
    static XDGInhibiter xdgi;
    if (xdgi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                xdgi.inhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "XDG inhibiter not valid." << std::endl;
    }
#endif

exit:
    return;

inhibitok:
    this->setStatus(targetStatus);
    goto exit;
}

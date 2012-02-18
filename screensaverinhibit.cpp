#include "screensaverinhibit.h"
#include "freedesktopinhibiter.h"
#include "xdginhibiter.h"

#include <QDebug>

ScreenSaverInhibit::ScreenSaverInhibit(QObject *parent) :
    QObject(parent)
{
    this->setStatus(SCREENSAVER_NON_INHIBITED);
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
    this->setScreenSaverDesinhibited();
}

void ScreenSaverInhibit::dontAllowScreenSaver()
{
    this->setScreenSaverInhibited();
}

void ScreenSaverInhibit::setScreenSaverInhibited()
{
    qDebug() << "Screensaver inhibition allowed.";
    this->switchScreenSaverInhibition(SCREENSAVER_INHIBITED);
}

void ScreenSaverInhibit::setScreenSaverDesinhibited()
{
    qDebug() << "Screensaver inhibition not allowed.";
    this->switchScreenSaverInhibition(SCREENSAVER_NON_INHIBITED);
}

void ScreenSaverInhibit::switchScreenSaverInhibition(ScreenSaverStatus targetStatus)
{
    qDebug() << "Screensaver target status:" << targetStatus;

#ifdef Q_OS_UNIX
    static XDGInhibiter xdgi;
    if (xdgi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                xdgi.inhibit();
                break;
            case SCREENSAVER_NON_INHIBITED:
                xdgi.desinhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "XDG inhibiter not valid." << std::endl;
    }
#endif

#ifdef HAVE_DBUS
    static FreedesktopInhibiter fdi;
    if(fdi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                fdi.inhibit();
                break;
            case SCREENSAVER_NON_INHIBITED:
                fdi.desinhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "Freedesktop inhibiter not valid." << std::endl;
    }
#endif

#ifdef Q_WS_WIN
#endif

exit:
    return;

inhibitok:
    this->setStatus(targetStatus);
    goto exit;
}

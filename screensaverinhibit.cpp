/* vim: set et ts=4 sw=4: */

#include "screensaverinhibit.h"
#include "freedesktopinhibiter.h"
#include "xdginhibiter.h"
#include "windowsinhibiter.h"
#include "osxinhibiter.h"

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

/* Mac OSX will be reported as UNIX and somehow, executing xdg-screensaver
   does not fail */
#if defined Q_OS_UNIX && !defined Q_WS_MAC
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

/* QtDBus is available under Mac but not working */
#if defined HAVE_DBUS && !defined Q_WS_MAC
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
    static WindowsInhibiter wi;
    if(wi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                wi.inhibit();
                break;
            case SCREENSAVER_NON_INHIBITED:
                wi.desinhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "Windows inhibiter not valid." << std::endl;
    }
#endif

#ifdef Q_WS_MAC
    static OSXInhibiter osxi;
    if(osxi.canHandle()) {
        switch(targetStatus) {
            case SCREENSAVER_INHIBITED:
                osxi.inhibit();
                break;
            case SCREENSAVER_NON_INHIBITED:
                osxi.desinhibit();
                break;
        }
        goto inhibitok;
    } else {
        std::cerr << "OSX inhibiter not valid." << std::endl;
    }
#endif

exit:
    return;

inhibitok:
    this->setStatus(targetStatus);
    goto exit;
}

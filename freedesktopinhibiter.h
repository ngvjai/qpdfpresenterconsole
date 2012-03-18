/* vim: set et ts=4 sw=4: */

#ifndef FREEDESKTOPINHIBITER_H
#define FREEDESKTOPINHIBITER_H

#include "app.h"
#include "screensaverinhibiters.h"

#include <QApplication>
#ifdef HAVE_DBUS
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>

class FreedesktopInhibiter : private ScreenSaverInhibiters
{
private:
    QDBusInterface *dbus;
    int cookie;

public:
    explicit FreedesktopInhibiter(void);
    ~FreedesktopInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();

};
#endif

#endif

#ifndef FREEDESKTOPINHIBITER_H
#define FREEDESKTOPINHIBITER_H

#include "screensaverinhibiters.h"

#include <QApplication>
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

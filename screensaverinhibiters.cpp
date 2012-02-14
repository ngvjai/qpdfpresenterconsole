#include "screensaverinhibiters.h"

ScreenSaverInhibiters::ScreenSaverInhibiters(QObject *parent) :
    QObject(parent)
{
}

void ScreenSaverInhibiters::FreedesktopInhibiter()
{
#ifdef HAVE_DBUS

#endif

    throw 0;
    return;
}

void ScreenSaverInhibiters::GnomeInhibiter()
{
#ifdef HAVE_DBUS
    QDBusConnection dbus = QDBusConnection::sessionBus();
#endif

    throw 0;
    return;
}

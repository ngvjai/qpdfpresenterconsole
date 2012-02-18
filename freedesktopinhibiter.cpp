#include "freedesktopinhibiter.h"

FreedesktopInhibiter::FreedesktopInhibiter(void)
{
    this->dbus = new QDBusInterface("org.freedesktop.ScreenSaver", "/ScreenSaver", "org.freedesktop.ScreenSaver");
}

FreedesktopInhibiter::~FreedesktopInhibiter()
{
    delete this->dbus;
}

bool FreedesktopInhibiter::canHandle()
{
    return this->dbus->isValid();
}

void FreedesktopInhibiter::inhibit()
{
    QDBusReply<int> cookie;
    cookie = this->dbus->call(QString("Inhibit"), QApplication::applicationName(), "Entering presentation mode");
    this->cookie = cookie;
}

void FreedesktopInhibiter::desinhibit()
{
    this->dbus->call(QString("Uninhibit"), this->cookie);
}

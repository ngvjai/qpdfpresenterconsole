/* vim: set et ts=4 sw=4: */

#include "osxinhibiter.h"
#include <QDebug>

#ifdef Q_WS_MAC
#include <CoreServices/CoreServices.h>

OSXInhibiter::OSXInhibiter()
{
    this->timer = new QTimer(this);
    QObject::connect(this->timer, SIGNAL(timeout()), SLOT(sendSystemNotification()));
}

bool OSXInhibiter::canHandle()
{
    return true;
}

void OSXInhibiter::inhibit()
{
// see http://developer.apple.com/library/mac/#qa/qa1160/_index.html
    this->timer->start(30000);
    this->sendSystemNotification();
}

void OSXInhibiter::desinhibit()
{
    this->timer->stop();
}

void OSXInhibiter::sendSystemNotification()
{
// simulating user activity: keyboard, mouse, etc.
    UpdateSystemActivity(UsrActivity);
}
#endif

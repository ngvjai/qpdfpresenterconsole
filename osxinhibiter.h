/* vim: set et ts=4 sw=4: */

#ifndef OSXINHIBITER_H
#define OSXINHIBITER_H

#include "screensaverinhibiters.h"
#include <QTimer>

class OSXInhibiter : public ScreenSaverInhibiters
{
// needed for slots, otherwise parent will handle them
    Q_OBJECT

    QTimer *timer;

public:
    explicit OSXInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();

public slots:
    void sendSystemNotification(void);
};

#endif // OSXINHIBITER_H

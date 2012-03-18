/* vim: set et ts=4 sw=4: */

#ifndef WINDOWSINHIBITER_H
#define WINDOWSINHIBITER_H

#include "screensaverinhibiters.h"

#ifdef Q_WS_WIN
class WindowsInhibiter : public ScreenSaverInhibiters
{
    void *previousState;
public:
    explicit WindowsInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();
};
#endif

#endif // WINDOWSINHIBITER_H

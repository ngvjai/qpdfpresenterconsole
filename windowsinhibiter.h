#ifndef WINDOWSINHIBITER_H
#define WINDOWSINHIBITER_H

#include "screensaverinhibiters.h"

#ifdef Q_WS_WIN
class WindowsInhibiter : public ScreenSaverInhibiters
{
public:
    explicit WindowsInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();
};
#endif

#endif // WINDOWSINHIBITER_H

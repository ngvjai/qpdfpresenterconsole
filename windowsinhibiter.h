/* vim: set et ts=4 sw=4: */

#ifndef WINDOWSINHIBITER_H
#define WINDOWSINHIBITER_H

#include "screensaverinhibiters.h"

class WindowsInhibiter : public ScreenSaverInhibiters
{
    void *previousState;
public:
    explicit WindowsInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();
};

#endif // WINDOWSINHIBITER_H

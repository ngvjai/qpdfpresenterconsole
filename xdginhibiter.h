#ifndef XDGINHIBITER_H
#define XDGINHIBITER_H

#include "screensaverinhibiters.h"

class XDGInhibiter : public ScreenSaverInhibiters
{
public:
    explicit XDGInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();
};

#endif // XDGINHIBITER_H

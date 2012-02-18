#ifndef XDGINHIBITER_H
#define XDGINHIBITER_H

#include "screensaverinhibiters.h"
#include <QWidget>

class XDGInhibiter : public ScreenSaverInhibiters
{
public:
    explicit XDGInhibiter();
    virtual bool canHandle();
    virtual void inhibit();
    virtual void desinhibit();

    QString getWindowId(void);
};

#endif // XDGINHIBITER_H

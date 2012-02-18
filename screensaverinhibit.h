#ifndef SCREENSAVERINHIBIT_H
#define SCREENSAVERINHIBIT_H

#include <QObject>
#include <iostream>
#include "app.h"
#include "screensaverinhibiters.h"

class ScreenSaverInhibit : public QObject
{
    Q_OBJECT
    bool canInhibit;
    ScreenSaverStatus status;
    void setCanInhibit(bool);
    bool getCanInhibit(void);
    void setStatus(ScreenSaverStatus status);
    ScreenSaverStatus getStatus(void);

public:
    explicit ScreenSaverInhibit(QObject *parent = 0);
    void handleScreenSaverInhibition(void);
    void switchScreenSaverInhibition(ScreenSaverStatus targetStatus);
    void setScreenSaverInhibited();
    void setScreenSaverDesinhibited();

signals:

public slots:
    void allowScreenSaver(void);
    void dontAllowScreenSaver(void);

};

#endif // SCREENSAVERINHIBIT_H

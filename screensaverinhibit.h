/* vim: set et ts=4 sw=4: */

#ifndef SCREENSAVERINHIBIT_H
#define SCREENSAVERINHIBIT_H

#include <QObject>
#include <iostream>
#include "app.h"
#include "screensaverinhibiters.h"

class ScreenSaverInhibit : public QObject
{
    Q_OBJECT
    ScreenSaverStatus status;
    void setStatus(ScreenSaverStatus status);
    ScreenSaverStatus getStatus(void);

public:
    explicit ScreenSaverInhibit(QObject *parent = 0);
    void switchScreenSaverInhibition(ScreenSaverStatus targetStatus);
    void setScreenSaverInhibited();
    void setScreenSaverDesinhibited();

signals:

public slots:
    void allowScreenSaver(void);
    void dontAllowScreenSaver(void);

};

#endif // SCREENSAVERINHIBIT_H

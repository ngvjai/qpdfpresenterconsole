/* vim: set et ts=4 sw=4: */

#ifndef SCREENSAVERINHIBITERS_H
#define SCREENSAVERINHIBITERS_H

#include <QObject>

enum ScreenSaverStatus {
    SCREENSAVER_INHIBITED,
    SCREENSAVER_NON_INHIBITED
};

class ScreenSaverInhibiters : public QObject
{
    Q_OBJECT

public:
    explicit ScreenSaverInhibiters(QObject *parent = 0);
    virtual bool canHandle(void) = 0;
    virtual void inhibit(void) = 0;
    virtual void desinhibit(void) = 0;

signals:

public slots:

};

#endif // SCREENSAVERINHIBITERS_H

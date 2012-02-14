#ifndef SCREENSAVERINHIBITERS_H
#define SCREENSAVERINHIBITERS_H

#include <QObject>
#include "app.h"
#ifdef HAVE_DBUS
#include <QtDBus/QDBusConnection>
#endif

class ScreenSaverInhibiters : public QObject
{
    Q_OBJECT

public:
    explicit ScreenSaverInhibiters(QObject *parent = 0);
    static void FreedesktopInhibiter();
    static void GnomeInhibiter();

signals:

public slots:

};

#endif // SCREENSAVERINHIBITERS_H

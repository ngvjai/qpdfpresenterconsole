/* vim: set et ts=4 sw=4: */

#ifndef PRESENTATIONTIMER_H
#define PRESENTATIONTIMER_H

#include <QObject>
#include <QTimer>
#include <QTimerEvent>
#include "parameters.h"

class PresentationTimer : public QObject
{
    Q_OBJECT

    Parameters *params;

    QTimer *timerLength;
    int presentationLength;
    int presentationEmergency;
    int timerInterval;
    int timerId;
    bool started;

public:
    explicit PresentationTimer(QObject *parent = 0, Parameters *params = 0);
    int getPresentationHours();
    int getPresentationMinutes();
    int getPresentationSeconds();
    int getEmergencyHours();
    int getEmergencyMinutes();
    int getEmergencySeconds();
    bool isCritical();
    bool isFinished();
    void launch();
    void initCounter();

signals:
    void timerChanged();

public slots:
    void startCounterIfNeeded();
    void resetCounter();
    void timerEvent(QTimerEvent *);

};

#endif // PRESENTATIONTIMER_H

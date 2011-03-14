#include "presentationtimer.h"

PresentationTimer::PresentationTimer(QObject *parent, Parameters *params) :
    QObject(parent)
{
    this->params = params;
    this->started = false;
    this->timerLength = new QTimer(this);
    this->initCounter();
}

void PresentationTimer::initCounter()
{
    this->presentationEmergency = this->params->getPresentationEmergency();
    this->presentationLength = this->params->getPresentationLength();
    this->timerInterval = 1000;

    emit timerChanged();
}

void PresentationTimer::timerEvent(QTimerEvent *timer)
{
    this->presentationLength -= this->timerInterval;

    if (this->presentationLength <= 0) {
        this->killTimer(timer->timerId());
    }

    emit timerChanged();
}

int PresentationTimer::getPresentationHours()
{
    return this->presentationLength / (1000*60*60);
}

int PresentationTimer::getPresentationMinutes()
{
    return (this->presentationLength % (1000*60*60)) / (1000*60);
}

int PresentationTimer::getPresentationSeconds()
{
    return ((this->presentationLength % (1000*60*60)) % (1000*60)) / 1000;
}

int PresentationTimer::getEmergencyHours()
{
    return this->presentationEmergency / (1000*60*60);
}

int PresentationTimer::getEmergencyMinutes()
{
    return (this->presentationEmergency % (1000*60*60)) / (1000*60);
}

int PresentationTimer::getEmergencySeconds()
{
    return ((this->presentationEmergency % (1000*60*60)) % (1000*60)) / 1000;
}

bool PresentationTimer::isCritical()
{
    return (this->presentationLength <= this->presentationEmergency);
}

void PresentationTimer::launch()
{
    this->timerId = this->startTimer(this->timerInterval);
    this->started = true;
}

void PresentationTimer::startCounterIfNeeded()
{
    if (!this->started) {
        this->launch();
    }
}

void PresentationTimer::resetCounter()
{
    this->started = false;
    this->killTimer(this->timerId);
    this->initCounter();
}

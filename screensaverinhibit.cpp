#include "screensaverinhibit.h"

ScreenSaverInhibit::ScreenSaverInhibit(QObject *parent) :
    QObject(parent)
{
    this->canInhibit = false;
}

void ScreenSaverInhibit::setCanInhibit(bool v)
{
    this->canInhibit = v;
}

bool ScreenSaverInhibit::getCanInhibit()
{
    return this->canInhibit;
}

void ScreenSaverInhibit::allowScreenSaver()
{
    this->setCanInhibit(true);
}

void ScreenSaverInhibit::dontAllowScreenSaver()
{
    this->setCanInhibit(false);
}

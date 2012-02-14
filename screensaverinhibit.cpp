#include "screensaverinhibit.h"

ScreenSaverInhibit::ScreenSaverInhibit(QObject *parent) :
    QObject(parent)
{
    this->canInhibit = false;
}

void ScreenSaverInhibit::setCanInhibit(bool v)
{
    this->canInhibit = v;
    this->handleScreenSaverInhibition();
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

void ScreenSaverInhibit::handleScreenSaverInhibition()
{
    if (this->getCanInhibit()) {
        std::cerr << "Screen saver allowed, no inhibition." << std::endl;
        return;
    }

    try {
        ScreenSaverInhibiters::FreedesktopInhibiter();
    } catch (...) {
        std::cerr << "No support for Freedesktop screensaver inhibition" << std::endl;
    }

    try {
        ScreenSaverInhibiters::GnomeInhibiter();
    } catch (...) {
        std::cerr << "No support for Gnome screensaver inhibition" << std::endl;
    }

    return;
}

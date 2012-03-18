/* vim: set et ts=4 sw=4: */

#include "windowsinhibiter.h"
#include <QDebug>

/**
 * confere include/winver.h in mingw:
 * "If you need Win32 API features newer the Win95 and WinNT then you must
 * define WINVER before including windows.h or any other method of including
 * the windef.h header."
 **/
#define WINVER 0x0500
#include <windows.h>

WindowsInhibiter::WindowsInhibiter()
{
}

bool WindowsInhibiter::canHandle()
{
    return true;
}

void WindowsInhibiter::inhibit()
{
    this->previousState = (void *)SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
    if (this->previousState == NULL) {
        qDebug() << "SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED) returned NULL";
    } else {
        qDebug() << "SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED) returned something NOT NULL (good!)";
    }
}

void WindowsInhibiter::desinhibit()
{
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
}

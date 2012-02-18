#include "windowsinhibiter.h"

#ifdef Q_WS_WIN
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
}

void WindowsInhibiter::inhibit()
{
}

void WindowsInhibiter::desinhibit()
{
}
#endif

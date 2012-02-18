#include "windowsinhibiter.h"

#ifdef Q_WS_WIN
#include <windows.h>
#include <winbase.h>

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

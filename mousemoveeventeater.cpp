/* vim: set et ts=4 sw=4: */

#include "mousemoveeventeater.h"

MouseMoveEventEater::MouseMoveEventEater(QObject *parent) :
    QObject(parent)
{
}

bool MouseMoveEventEater::eventFilter(QObject *obj, QEvent *ev)
{
    if (ev->type() == QEvent::MouseMove) {
        return false;
    }
}

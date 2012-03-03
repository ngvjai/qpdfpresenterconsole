#ifndef MOUSEMOVEEVENTEATER_H
#define MOUSEMOVEEVENTEATER_H

#include <QObject>
#include <QEvent>
#include <QMouseEvent>

class MouseMoveEventEater : public QObject
{
    Q_OBJECT
public:
    explicit MouseMoveEventEater(QObject *parent = 0);

signals:

public slots:

protected:
    bool eventFilter(QObject *obj, QEvent *ev);

};

#endif // MOUSEMOVEEVENTEATER_H

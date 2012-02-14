#ifndef SCREENSAVERINHIBIT_H
#define SCREENSAVERINHIBIT_H

#include <QObject>
#include <QtDBus>

class ScreenSaverInhibit : public QObject
{
    Q_OBJECT
    bool canInhibit;
    void setCanInhibit(bool);
    bool getCanInhibit(void);

public:
    explicit ScreenSaverInhibit(QObject *parent = 0);

signals:

public slots:
    void allowScreenSaver(void);
    void dontAllowScreenSaver(void);

};

#endif // SCREENSAVERINHIBIT_H

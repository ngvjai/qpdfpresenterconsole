#ifndef MAINSCREENPDFVIEW_H
#define MAINSCREENPDFVIEW_H

#include <QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QTimer>
#include "pdfmodel.h"
#include "parameters.h"
#include "presentationtimer.h"

class MainScreenPdfView : public QMainWindow
{
    Q_OBJECT
    PDFModel *modele;
    Parameters *params;
    QLabel *slides;
    QLabel *timer;
    QLabel *currentDate;
    QLabel *emergencyDate;

    QLabel *currentSlide;
    QLabel *nextSlide;
    QLabel *beamerNote;

    PresentationTimer *pTimer;
    bool maximized;

public:
    explicit MainScreenPdfView(QWidget *parent = 0, PDFModel *modele = 0, Parameters *params = 0, PresentationTimer *timer = 0);

signals:
    void keyPressed(QKeyEvent *e);
    void presentationStarted();

public slots:
    void keyReleaseEvent(QKeyEvent *ev);
    void timerUpdated(void);
    void updateView(void);
    void timerEvent(QTimerEvent *ev);
    void resizeEvent(QResizeEvent *ev);

};

#endif // MAINSCREENPDFVIEW_H

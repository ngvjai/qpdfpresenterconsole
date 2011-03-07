#ifndef MAINSCREENPDFVIEW_H
#define MAINSCREENPDFVIEW_H

#include <QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QLabel>
#include <QTimer>
#include "pdfmodel.h"
#include "parameters.h"

class MainScreenPdfView : public QMainWindow
{
    Q_OBJECT
    PDFModel *modele;
    Parameters *params;
    QLabel *slides;
    QLabel *timer;

    QLabel *currentSlide;
    QLabel *nextSlide;

    QTimer *timerLength;
    int presentationLength;
    int presentationEmergency;
    int timerInterval;

    void updateTimerView(void);

public:
    explicit MainScreenPdfView(QWidget *parent = 0, PDFModel *modele = 0, Parameters *params = 0);

signals:

public slots:
    void keyReleaseEvent(QKeyEvent *ev);
    void timerEvent(QTimerEvent *timer);
    void updateView(void);

};

#endif // MAINSCREENPDFVIEW_H

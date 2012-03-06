#ifndef MAINSCREENPDFVIEW_H
#define MAINSCREENPDFVIEW_H

#include <QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QLabel>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include "pdfmodel.h"
#include "parameters.h"
#include "presentationtimer.h"
#include "optionsdialog.h"
#include "screensaverinhibit.h"

class MainScreenPdfView : public QMainWindow
{
    Q_OBJECT
    PDFModel *modele;
    Parameters *params;
    ScreenSaverInhibit *screensaver;
    QLabel *slides;
    QLabel *timer;
    QLabel *currentDate;
    QLabel *emergencyDate;

    QLabel *currentSlide;
    QLabel *nextSlide;
    QLabel *beamerNote;

    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextAction;
    QAction *previousAction;
    QToolBar *mediabar;
    QToolBar *controlbar;

    OptionsDialog *options;
    PresentationTimer *pTimer;
    bool maximized;
    float f1, f2;

public:
    explicit MainScreenPdfView(QWidget *parent = 0, PDFModel *modele = 0, Parameters *params = 0, PresentationTimer *timer = 0, ScreenSaverInhibit *screensaver = 0);

protected:
    void wheelEvent(QWheelEvent *ev);

signals:
    void keyPressed(QKeyEvent *e);
    void mousePressed(QMouseEvent *e);
    void presentationStarted();
    void presentationMode();
    void desktopMode();
    void mouseWheelEvent(QWheelEvent *ev);

public slots:
    void keyReleaseEvent(QKeyEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void timerUpdated(void);
    void resetPresentationTimer(void);
    void updateView(void);
    void timerEvent(QTimerEvent *ev);
    void resizeEvent(QResizeEvent *ev);
    void moveToScreen(void);

    /* toolbar slots */
    void leaveApplication();
    void switchDesktopPresentationMode();
    void showHelpDialog();
    void showGotoDialog();
    void showOptionsDialog();

};

#endif // MAINSCREENPDFVIEW_H

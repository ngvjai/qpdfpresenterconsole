/* vim: set et ts=4 sw=4: */

#ifndef MAINSCREENPDFVIEW_H
#define MAINSCREENPDFVIEW_H

#include <QMainWindow>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QLabel>
#include <QtGui/QTextEdit>
#include <QAction>
#include <QToolBar>
#include <QTimer>
#include <QSlider>
#include "pdfmodel.h"
#include "parameters.h"
#include "presentationtimer.h"
#include "optionsdialog.h"
#include "screensaverinhibit.h"
#include "helpdialog.h"

class MainScreenPdfView : public QMainWindow
{
    Q_OBJECT
    PDFModel *modele;
    Parameters *params;
    ScreenSaverInhibit *screensaver;
    QLabel *slides;
    QLabel *nextslides;
    QLabel *timer;
    QLabel *currentDate;
    QLabel *emergencyDate;

    QLabel *currentSlide;
    QLabel *nextSlide;
    QTextEdit *beamerNote;

    QAction *playAction;
    QAction *pauseAction;
    QAction *stopAction;
    QAction *nextAction;
    QAction *previousAction;
    QToolBar *mediabar;
    QToolBar *controlbar;

    QSlider *positionSlider;
    QSlider *volumeSlider;
    bool canSlidePosition;

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

    /* media player */
    void updateMediaPlayerTime(qint64 time);
    void updateMediaPlayerPosition(float position);

    void lockSlider();
    void unlockSlider();
    void userSeekBackward();
    void userSeekForward();
    void userChangePosition(int value);
    void userChangeVolume(int value);

};

#endif // MAINSCREENPDFVIEW_H

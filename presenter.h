/* vim: set et ts=4 sw=4: */

#ifndef PRESENTER_H
#define PRESENTER_H

#include <QApplication>
#include <QDesktopWidget>
#include <QMessageBox>
#include <QObject>
#include "mainscreenpdfview.h"
#include "textannot.h"
#include "presenterpdf.h"
#include "pdfmodel.h"
#include "screensaverinhibit.h"
#include "app.h"

class Presenter : public QApplication
{
    Q_OBJECT

    Parameters *params;
    PresentationTimer *presentationTimer;
    ScreenSaverInhibit *screensaverinhibiter;
    PDFModel *pdf;
    MainScreenPdfView *mainScreen;
    PresenterPdf *presenterPdf;
    bool ready;

public:
    Presenter(int &argc, char **argv);
    bool isReady();

protected:
    bool event(QEvent *ev);

signals:

public slots:

};

#endif // PRESENTER_H

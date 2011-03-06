#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>
#include <QObject>
#include <stdlib.h>

#include "mainscreenpdfview.h"
#include "presenterpdf.h"
#include "pdfmodel.h"
#include "app.h"

int main(int argc, char *argv[])
{
    QString pdfFile(argv[1]);
    QApplication app(argc, argv);
    QDesktopWidget *desktop = app.desktop();
    PDFModel pdf((QObject*)desktop, pdfFile);

    if(!pdf.pdfLoaded()) {
        QString err = "File '%1' not found. Cannot continue.";
        QString errStr = err.arg(pdfFile);
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr(errStr.toStdString().c_str())
                              );
        return EXIT_FAILURE;
    }

    MainScreenPdfView mainScreen(0, &pdf);
    PresenterPdf presenterPdf(0, &pdf);

    bool hasMultipleScreens = (desktop->screenCount() > 1) ? true : false;

    if (!hasMultipleScreens) {
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr("No multiscreen enabled. Quitting.")
                              );
        return EXIT_SUCCESS;
    }

    pdf.gotoFirstPage();
    presenterPdf.showFullScreen();
    mainScreen.showFullScreen();

    return app.exec();
}

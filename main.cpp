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
    QCoreApplication::setApplicationName(appname);
    QCoreApplication::setApplicationVersion(appvers);
    QApplication app(argc, argv);

    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(SHORTNAME) + "_" + locale, QString(DATADIR));
    app.installTranslator(&translator);

    QDesktopWidget *desktop = app.desktop();
    Parameters params((QObject*)desktop);

    bool hasMultipleScreens = (desktop->screenCount() > 1) ? true : false;
    if (!hasMultipleScreens) {
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr("No multiscreen enabled. Quitting.")
                              );

        exit(EXIT_FAILURE);
    }

    PresentationTimer presentationTimer(0, &params);

    PDFModel pdf((QObject*)desktop, &params, &presentationTimer);
    if(!pdf.pdfLoaded()) {
        QString err = QObject::tr("File '%1' not found. Cannot continue.")
                    .arg(pdf.getPdfFileName());

        QMessageBox::critical(0,
                              APPNAME,
                              err
                              );

        exit(EXIT_FAILURE);
    }

    MainScreenPdfView mainScreen(0, &pdf, &params, &presentationTimer);
    PresenterPdf presenterPdf(0, &pdf, &params);

    pdf.gotoOpenPage();
    mainScreen.setFocus();
    presenterPdf.showFullScreen();
    mainScreen.showFullScreen();

    return app.exec();
}

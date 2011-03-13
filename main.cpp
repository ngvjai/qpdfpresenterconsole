#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QMessageBox>
#include <QObject>

#include <stdlib.h>
#include <stdio.h>

#include "mainscreenpdfview.h"
#include "presenterpdf.h"
#include "pdfmodel.h"
#include "app.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf8"));
    QString locale = QLocale::system().name();
    QTranslator translator;
    translator.load(QString(SHORTNAME) + "_" + locale);
    app.installTranslator(&translator);

    QStringList arguments = QCoreApplication::arguments();

    QDesktopWidget *desktop = app.desktop();
    Parameters params((QObject*)desktop);
    params.setParameters(arguments);

    PresentationTimer presentationTimer(0, &params);

    PDFModel pdf((QObject*)desktop, &params, &presentationTimer);
    if(!pdf.pdfLoaded()) {
        QString err = "File '%1' not found. Cannot continue.";
        QString errStr = err.arg(pdf.getPdfFileName());
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr(errStr.toStdString().c_str())
                              );
        return EXIT_FAILURE;
    }

    MainScreenPdfView mainScreen(0, &pdf, &params, &presentationTimer);
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
    mainScreen.showFullScreen();

    return app.exec();
}

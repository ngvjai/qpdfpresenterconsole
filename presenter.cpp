/* vim: set et ts=4 sw=4: */

#include "presenter.h"

Presenter::Presenter(int &argc, char **argv)
    : QApplication(argc, argv)
{
    QCoreApplication::setOrganizationName(orgname);
    QCoreApplication::setApplicationName(appname);
    QCoreApplication::setApplicationVersion(appvers);

    QString locale = QLocale::system().name();
    this->translator = new QTranslator(this);
    this->translator->load(QString(SHORTNAME) + "_" + locale, QString(DATADIR));
    this->installTranslator(this->translator);

    this->params = new Parameters(this);

    bool hasMultipleScreens = (this->desktop()->screenCount() > 1) ? true : false;
    if (this->params->getCheckMultiDisplay() && !hasMultipleScreens) {
        QMessageBox::critical(0,
                              QObject::tr(APPNAME),
                              QObject::tr("No multiscreen enabled. Quitting.")
                              );
        this->ready = false;
    } else {
        this->ready = true;
        this->presentationTimer = new PresentationTimer(this, this->params);
        this->screensaverinhibiter = new ScreenSaverInhibit(this);

        this->pdf = new PDFModel(this, this->params, this->presentationTimer);
        this->presenterPdf = new PresenterPdf(this->desktop(), this->pdf, this->params);
        this->mainScreen = new MainScreenPdfView(this->desktop(), this->pdf, this->params, this->presentationTimer, this->screensaverinhibiter);
        this->mainScreen->setFocus();

        QObject::connect(this, SIGNAL(aboutToQuit()), this->params, SLOT(saveSettingsOnClose()));
    }

    if (this->params->getPdfFileName().isEmpty()) {
        QString fileName = QFileDialog::getOpenFileName(this->mainScreen,
             tr("Open PDF file"), "", tr("PDF Files (*.pdf)"));
        if (!fileName.isEmpty()) {
            this->params->setPdfFileName(fileName);
            this->pdf->setPdfFileName(fileName);
        }
    }
}

bool Presenter::isReady()
{
    return this->ready;
}

bool Presenter::event(QEvent *ev)
{
    bool eaten;
    switch (ev->type()) {
        case QEvent::FileOpen:
            {
                this->pdf->setPdfFileName(static_cast<QFileOpenEvent *>(ev)->file());
                QFileDialog* open = this->mainScreen->findChild<QFileDialog*>();
                if (open) {
                    open->close();
                }
                eaten = true;
            }
            break;
        default:
            eaten = QApplication::event(ev);
            break;
    }

    return eaten;
}

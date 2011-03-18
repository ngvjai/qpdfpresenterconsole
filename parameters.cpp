#include "app.h"
#include "parameters.h"
#include <QDebug>
#include <iostream>

Parameters::Parameters(QObject *parent) :
    QObject(parent)
{
    static const struct QCommandLineConfigEntry conf[] =
      {
        // { QCommandLine::Option, 'v', "verbose", "Verbose level (0-3)", QCommandLine::Mandatory },
        // { QCommandLine::Switch, 'l', "list", "Show a list", QCommandLine::Optional },
        // { QCommandLine::Param, QChar(), "target", "The target", QCommandLine::Mandatory },
        // { QCommandLine::Param, QChar(), "source", "The sources", QCommandLine::MandatoryMultiple },
        {
            QCommandLine::Option, 'd', "duration",
            QObject::tr("Duration of the presentation in minutes. Default: %1.").arg(DEFAULT_DURATION),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'e', "emergency",
            QObject::tr("Timer emergency in minutes. When reached, timer becomes red. Default: %1.").arg(DEFAULT_EMERGENCY),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'w', "slides-width",
            QObject::tr("Main slide width on computer's main screen, in percent. Default: %1.").arg(DEFAULT_SLIDESWIDTH),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'p', "page",
            QObject::tr("Starting page number. PDF will be opened at this page. Default: %1.").arg(DEFAULT_PAGE),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'm', "mainscreen",
            QObject::tr("Main screen identifier. This is where augmented presentation will be. Default: %1.").arg(DEFAULT_MAINSCREEN),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 's', "projectorscreen",
            QObject::tr("Projector screen identifier. This is where classical presentation will be. Default: %1.").arg(DEFAULT_PROJECTORSCREEN),
            QCommandLine::Optional
        },
        {
            QCommandLine::Param, QChar(), "file",
            QObject::tr("PDF file of the presentation."),
            QCommandLine::Optional
        },
        QCOMMANDLINE_CONFIG_ENTRY_END
      };

    cmdline = new QCommandLine(this);
    cmdline->setConfig(conf);
    cmdline->enableVersion(true); // enable -v // --version
    cmdline->enableHelp(true); // enable -h / --help

    connect(cmdline, SIGNAL(switchFound(const QString &)),
            this, SLOT(switchFound(const QString &)));
    connect(cmdline, SIGNAL(optionFound(const QString &, const QVariant &)),
            this, SLOT(optionFound(const QString &, const QVariant &)));
    connect(cmdline, SIGNAL(paramFound(const QString &, const QVariant &)),
            this, SLOT(paramFound(const QString &, const QVariant &)));
    connect(cmdline, SIGNAL(parseError(const QString &)),
            this, SLOT(parseError(const QString &)));

    this->setDefaultParameters();
    cmdline->parse();
}

void Parameters::setDefaultParameters()
{
    this->setCurrentSlidePrcentWidth(DEFAULT_SLIDESWIDTH / 100.0);
    this->setPresentationLength(1000*60*DEFAULT_DURATION); /* in msecs */
    this->setPresentationEmergency(1000*60*DEFAULT_EMERGENCY);
    this->setOpenPage(DEFAULT_PAGE - 1);
    this->setMainScreenId(DEFAULT_MAINSCREEN);
    this->setProjectorScreenId(DEFAULT_PROJECTORSCREEN);
    this->setPdfFileName("");
}

void Parameters::setPresentationLength(int v)
{
    this->presentationLength = v;
}

int Parameters::getPresentationLength()
{
    return this->presentationLength;
}

void Parameters::setPresentationEmergency(int v)
{
    this->presentationEmergency = v;
}

int Parameters::getPresentationEmergency()
{
    return this->presentationEmergency;
}

void Parameters::setPdfFileName(QString v)
{
    this->pdfFileName = v;
}

QString Parameters::getPdfFileName()
{
    return this->pdfFileName;
}

void Parameters::setCurrentSlidePrcentWidth(float v)
{
    this->currentSlidePrcentWidth = v;
}

float Parameters::getCurrentSlidePrcentWidth()
{
    return this->currentSlidePrcentWidth;
}

void Parameters::setOpenPage(int v)
{
    this->openPage = v;
}

int Parameters::getOpenPage()
{
    return this->openPage;
}

void Parameters::setMainScreenId(int v)
{
    this->mainScreenId = v;
}

int Parameters::getMainScreenId()
{
    return this->mainScreenId;
}

void Parameters::setProjectorScreenId(int v)
{
    this->projectorScreenId = v;
}

int Parameters::getProjectorScreenId()
{
    return this->projectorScreenId;
}

void Parameters::switchFound(const QString &name)
{
    // qWarning() << "Switch:" << name;
}

void Parameters::optionFound(const QString &name, const QVariant &value)
{
     if (name == "emergency") {
         this->setPresentationEmergency(1000*60*(value.toInt()));
     }
     if (name == "duration") {
         this->setPresentationLength(1000*60*(value.toInt()));
     }
     if (name == "slides-width") {
         this->setCurrentSlidePrcentWidth(value.toInt() / 100.0);
     }
     if (name == "page") {
         this->setOpenPage(value.toInt() - 1);
     }
     if (name == "mainscreen") {
         this->setMainScreenId(value.toInt());
     }
     if (name == "projectorscreen") {
         this->setProjectorScreenId(value.toInt());
     }
}

void Parameters::paramFound(const QString &name, const QVariant &value)
{
     if (name == "file") {
         this->setPdfFileName(value.toString());
     }
}

void Parameters::parseError(const QString &error)
{
    std::cerr << error.toStdString() << std::endl;
    cmdline->showHelp(true, -1);
    QCoreApplication::quit();
}

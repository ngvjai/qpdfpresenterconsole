#include "app.h"
#include "parameters.h"
#include <QDebug>

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
            QCommandLine::Option, 's', "slides-width",
            QObject::tr("Main slide width on computer's main screen, in percent. Default: %1.").arg(DEFAULT_SLIDESWIDTH),
            QCommandLine::Optional
        },
        {
            QCommandLine::Param, QChar(), "file",
            QObject::tr("PDF file of the presentation"),
            QCommandLine::Mandatory
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
}

void Parameters::paramFound(const QString &name, const QVariant &value)
{
     if (name == "file") {
         this->setPdfFileName(value.toString());
     }
}

void Parameters::parseError(const QString &error)
{
     qWarning() << qPrintable(error);
     cmdline->showHelp(true, -1);
     QCoreApplication::quit();
}

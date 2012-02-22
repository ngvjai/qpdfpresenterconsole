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
            QCommandLine::Option, 'n', "beamernotes",
            QObject::tr("Enable support for Beamer notes. Default: %1.").arg(DEFAULT_BEAMERNOTES),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'l', "beamernotespart",
            QObject::tr("Which part of the slides for Beamer notes (left or right screen). Default: %1.").arg(DEFAULT_BEAMERNOTESPART),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 't', "textannot",
            QObject::tr("Use annotations from text file. Default: %1.").arg(DEFAULT_TEXTANNOT),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'c', "checkmultidisplay",
            QObject::tr("Check the presence of multiple screen. Default: %1.").arg(DEFAULT_CHECKMULTIDISPLAY),
            QCommandLine::Optional
        },
        {
            QCommandLine::Option, 'f', "configfile",
            QObject::tr("Config file to load parameters from. Any other command line parameter will be ignored. Default: %1.").arg(DEFAULT_CONFIGFILE),
            QCommandLine::Optional
        },
        {
            QCommandLine::Param, QChar(), "file",
            QObject::tr("PDF file of the presentation."),
            QCommandLine::Optional
        },
        QCOMMANDLINE_CONFIG_ENTRY_END
      };

    this->setDefaultParameters();
    this->loadSettingsOnStartup();

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

    cmdline->parse();

    if (!this->getConfigFile().isEmpty()) {
        this->loadConfigFile();
    }
}

void Parameters::loadSettingsOnStartup()
{
    QSettings settings;
    this->loadSettings(settings);
}

void Parameters::loadConfigFile()
{
    QSettings settings(this->getConfigFile(), QSettings::IniFormat);
    this->loadSettings(settings);
}

void Parameters::saveSettingsOnClose()
{
    if (this->getConfigFile().isEmpty()) {
        this->saveSettings();
    }
}

void Parameters::saveSettings()
{
    QSettings settings;

    settings.setValue("checkMultiDisplay", this->getCheckMultiDisplay());
    settings.setValue("presentationLength", this->getPresentationLength());
    settings.setValue("presentationEmergency", this->getPresentationEmergency());
    settings.setValue("currentSlidePrcentWidth", this->getCurrentSlidePrcentWidth());
    settings.setValue("openPage", this->getOpenPage());
    settings.setValue("mainScreenId", this->getMainScreenId());
    settings.setValue("projectorScreenId", this->getProjectorScreenId());
    settings.setValue("beamerNotes", this->getBeamerNotes());
    settings.setValue("beamerNotesPart", this->getBeamerNotesPart());
    settings.setValue("textAnnot", this->getTextAnnot());

    settings.sync();
}

void Parameters::loadSettings(QSettings &settings)
{
    settings.sync();

    this->setCheckMultiDisplay(
                settings.value("checkMultiDisplay", DEFAULT_CHECKMULTIDISPLAY).toBool());
    this->setPresentationLength(
                settings.value("presentationLength", DEFAULT_PRESENTATIONLENGTH).toInt());
    this->setPresentationEmergency(
                settings.value("presentationEmergency", DEFAULT_PRESENTATIONEMERGENCY).toInt());
    this->setCurrentSlidePrcentWidth(
                settings.value("currentSlidePrcentWidth", DEFAULT_CURRENTSLIDEPRCENTWIDTH).toFloat());
    this->setOpenPage(
                settings.value("openPage", DEFAULT_OPENPAGE).toInt());
    this->setMainScreenId(
                settings.value("mainScreenId", DEFAULT_MAINSCREEN).toInt());
    this->setProjectorScreenId(
                settings.value("projectorScreenId", DEFAULT_PROJECTORSCREEN).toInt());
    this->setBeamerNotes(
                settings.value("beamerNotes", DEFAULT_BEAMERNOTES).toBool());
    this->setBeamerNotesPart(
                settings.value("beamerNotesPart", DEFAULT_BEAMERNOTESPART).toString());
    this->setTextAnnot(
                settings.value("textAnnot", DEFAULT_TEXTANNOT).toBool());
}

void Parameters::setDefaultParameters()
{
    this->setCurrentSlidePrcentWidth(DEFAULT_CURRENTSLIDEPRCENTWIDTH);
    this->setPresentationLength(DEFAULT_PRESENTATIONLENGTH); /* in msecs */
    this->setPresentationEmergency(DEFAULT_PRESENTATIONEMERGENCY);
    this->setOpenPage(DEFAULT_OPENPAGE);
    this->setMainScreenId(DEFAULT_MAINSCREEN);
    this->setProjectorScreenId(DEFAULT_PROJECTORSCREEN);
    this->setBeamerNotes(DEFAULT_BEAMERNOTES);
    this->setBeamerNotesPart(DEFAULT_BEAMERNOTESPART);
    this->setTextAnnot(DEFAULT_TEXTANNOT);
    this->setCheckMultiDisplay(DEFAULT_CHECKMULTIDISPLAY);
    this->setConfigFile(DEFAULT_CONFIGFILE);
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
    emit mainScreenChanged();
}

int Parameters::getMainScreenId()
{
    return this->mainScreenId;
}

void Parameters::setProjectorScreenId(int v)
{
    this->projectorScreenId = v;
    emit projectorScreenChanged();
}

int Parameters::getProjectorScreenId()
{
    return this->projectorScreenId;
}

void Parameters::setBeamerNotes(bool v)
{
    this->beamerNotes = v;
    emit beamerNotesChanged();
}

bool Parameters::getBeamerNotes()
{
    return this->beamerNotes;
}

void Parameters::setBeamerNotesPart(QString v)
{
    this->beamerNotesPart = v;
    emit beamerNotesChanged();
}

QString Parameters::getBeamerNotesPart()
{
    return this->beamerNotesPart;
}

void Parameters::setTextAnnot(bool v, bool emitSignal)
{
    this->textAnnot = v;

    if (emitSignal) {
        emit this->textAnnotChanged();
    }
}

bool Parameters::getTextAnnot()
{
    return this->textAnnot;
}

void Parameters::setCheckMultiDisplay(bool v)
{
    this->checkMultiDisplay = v;
}

bool Parameters::getCheckMultiDisplay()
{
    return this->checkMultiDisplay;
}

void Parameters::setConfigFile(QString v)
{
    this->configfile = v;
}

QString Parameters::getConfigFile()
{
    return this->configfile;
}

void Parameters::switchFound(const QString &name)
{
    // qWarning() << "Switch:" << name;
}

void Parameters::optionFound(const QString &name, const QVariant &value)
{
    if (name == "configfile") {
        this->setConfigFile(value.toString());
    }
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
    if (name == "beamernotes") {
        this->setBeamerNotes(value.toBool());
    }
    if (name == "beamernotespart") {
        this->setBeamerNotesPart(value.toString());
        this->setBeamerNotes(true);
    }
    if (name == "textannot") {
        this->setTextAnnot(value.toBool());
    }
    if (name == "checkmultidisplay") {
        this->setCheckMultiDisplay(value.toBool());
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

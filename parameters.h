#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QCoreApplication>
#include <QSettings>
#include <QList>
#include "qcommandline.h"

class Parameters : public QObject
{
    Q_OBJECT

    QString pdfFileName;
    bool checkMultiDisplay;
    int presentationLength;
    int presentationEmergency;
    float currentSlidePrcentWidth;
    int openPage;
    int mainScreenId;
    int projectorScreenId;
    bool beamerNotes;
    QString beamerNotesPart;
    bool textAnnot;

    QString configfile;

    QCommandLine *cmdline;

public:
    explicit Parameters(QObject *parent = 0);

    void loadSettingsOnStartup();
    void saveSettings();
    void loadSettings(QSettings &settings);
    void loadConfigFile();

    void setPresentationLength(int v);
    void setPresentationEmergency(int v);
    void setPdfFileName(QString v);
    void setCurrentSlidePrcentWidth(float v);
    void setOpenPage(int v);
    void setMainScreenId(int v);
    void setProjectorScreenId(int v);
    void setBeamerNotes(bool v);
    void setBeamerNotesPart(QString v);
    void setDefaultParameters();
    void setTextAnnot(bool v, bool emitSignal = true);
    void setCheckMultiDisplay(bool v);
    void setConfigFile(QString v);

    int getPresentationLength();
    int getPresentationEmergency();
    QString getPdfFileName();
    float getCurrentSlidePrcentWidth();
    int getOpenPage();
    int getMainScreenId();
    int getProjectorScreenId();
    bool getBeamerNotes();
    QString getBeamerNotesPart();
    bool getTextAnnot();
    bool getCheckMultiDisplay();
    QString getConfigFile();

signals:
    void mainScreenChanged();
    void projectorScreenChanged();
    void beamerNotesChanged();
    void textAnnotChanged();

public slots:
    void parseError(const QString & error);
    void switchFound(const QString & name);
    void optionFound(const QString & name, const QVariant & value);
    void paramFound(const QString & name, const QVariant & value);
    void saveSettingsOnClose();
};

#endif // PARAMETERS_H

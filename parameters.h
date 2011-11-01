#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>
#include <QStringList>
#include <QVariant>
#include <QCoreApplication>
#include "qcommandline.h"

class Parameters : public QObject
{
    Q_OBJECT

    QString pdfFileName;
    int presentationLength;
    int presentationEmergency;
    float currentSlidePrcentWidth;
    int openPage;
    int mainScreenId;
    int projectorScreenId;
    bool beamerNotes;
    QString beamerNotesPart;
    bool textAnnot;

    QCommandLine *cmdline;

public:
    explicit Parameters(QObject *parent = 0);

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
};

#endif // PARAMETERS_H

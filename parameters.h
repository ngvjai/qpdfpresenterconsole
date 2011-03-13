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

    QCommandLine *cmdline;

    void setPresentationLength(int v);
    void setPresentationEmergency(int v);
    void setPdfFileName(QString v);
    void setCurrentSlidePrcentWidth(float v);
    void setDefaultParameters();

public:
    explicit Parameters(QObject *parent = 0);

    int getPresentationLength();
    int getPresentationEmergency();
    QString getPdfFileName();
    float getCurrentSlidePrcentWidth();

signals:

public slots:
    void parseError(const QString & error);
    void switchFound(const QString & name);
    void optionFound(const QString & name, const QVariant & value);
    void paramFound(const QString & name, const QVariant & value);
};

#endif // PARAMETERS_H

#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QObject>
#include <QStringList>

class Parameters : public QObject
{
    Q_OBJECT

    QString pdfFileName;
    int presentationLength;
    int presentationEmergency;
    float currentSlidePrcentWidth;

    void setPresentationLength(int v);
    void setPresentationEmergency(int v);
    void setPdfFileName(QString v);
    void setCurrentSlidePrcentWidth(float v);


public:
    explicit Parameters(QObject *parent = 0);
    void setParameters(QStringList params);

    int getPresentationLength();
    int getPresentationEmergency();
    QString getPdfFileName();
    float getCurrentSlidePrcentWidth();

signals:

public slots:

};

#endif // PARAMETERS_H

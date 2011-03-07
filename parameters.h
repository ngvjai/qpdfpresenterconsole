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

    void setPresentationLength(int v);
    void setPresentationEmergency(int v);
    void setPdfFileName(QString v);

public:
    explicit Parameters(QObject *parent = 0);
    void setParameters(QStringList params);

    int getPresentationLength();
    int getPresentationEmergency();
    QString getPdfFileName();

signals:

public slots:

};

#endif // PARAMETERS_H

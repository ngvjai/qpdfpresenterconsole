#include "parameters.h"

Parameters::Parameters(QObject *parent) :
    QObject(parent)
{

}

void Parameters::setParameters(QStringList params)
{

    this->setPresentationLength(1000*20*60); /* in msecs */
    this->setPresentationEmergency(1000*2*60);
    this->setPdfFileName(params.last());
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

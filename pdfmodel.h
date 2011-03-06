#ifndef PDFMODEL_H
#define PDFMODEL_H

#include <poppler-qt4.h>
#include <QObject>
#include <QtGui/QImage>
#include <QtGui/QDesktopWidget>
#include <QMessageBox>
#include "app.h"

class PDFModel : public QObject
{
    Q_OBJECT
    Poppler::Document* document;
    int firstPage;
    int currentPage;
    int lastPage;
    int dpiX;
    int dpiY;
    QRect projectorSize;
    QSizeF pageSize;
    float scaleFactorX;
    float scaleFactorY;

    QImage imgPreviousPage;
    QImage imgCurrentPage;
    QImage imgNextPage;

    int getCurrentPage();
    int getPreviousPage();
    int getNextPage();
    QImage renderPdfPage(int page);
    void renderPreviousPage();
    void renderCurrentPage();
    void renderNextPage();

public:
    explicit PDFModel(QObject *parent, QString file);
    ~PDFModel();
    bool pdfLoaded();
    void render();
    void gotoNextPage();
    void gotoPreviousPage();

    QImage getImgPreviousPage();
    QImage getImgCurrentPage();
    QImage getImgNextPage();

signals:
    void renderingChanged(void);

public slots:

};

#endif // PDFMODEL_H

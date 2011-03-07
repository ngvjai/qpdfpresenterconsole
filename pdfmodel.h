#ifndef PDFMODEL_H
#define PDFMODEL_H

#include <poppler-qt4.h>
#include <QObject>
#include <QtGui/QImage>
#include <QtGui/QDesktopWidget>
#include <QMessageBox>
#include "parameters.h"
#include "app.h"

class PDFModel : public QObject
{
    Q_OBJECT
    Poppler::Document *document;
    Parameters *params;
    int firstPage;
    int currentPage;
    int lastPage;
    int dpiX;
    int dpiY;
    QString pdfFileName;
    QRect projectorSize;
    QSizeF pageSize;
    float scaleFactorX;
    float scaleFactorY;

    QImage imgPreviousPage;
    QImage imgCurrentPage;
    QImage imgNextPage;

    void renderPreviousPage();
    void renderCurrentPage();
    void renderNextPage();

    void setPdfFileName(QString file);

public:
    explicit PDFModel(QObject *parent, Parameters *params);
    ~PDFModel();
    bool pdfLoaded();
    void render();
    void gotoNextPage();
    void gotoPreviousPage();
    void gotoFirstPage(void);
    void gotoLastPage(void);

    int getPreviousPage();
    int getCurrentPage();
    int getNextPage();
    int getLastPage();

    QImage renderPdfPage(int page);
    QImage renderPdfPage(int page, QSizeF scaleFactor);

    QImage getImgPreviousPage();
    QImage getImgCurrentPage();
    QImage getImgNextPage();

    QSizeF getPageSize();
    QSizeF getScaleFactor();

    QString getPdfFileName(void);

signals:
    void renderingChanged(void);

public slots:

};

#endif // PDFMODEL_H

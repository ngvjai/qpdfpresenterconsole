#ifndef PDFMODEL_H
#define PDFMODEL_H

#include <poppler-qt4.h>
#include <QObject>
#include <QtGui/QImage>
#include <QtGui/QDesktopWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include "parameters.h"
#include "presentationtimer.h"
#include "app.h"

class PDFModel : public QObject
{
    Q_OBJECT
    Poppler::Document *document;
    Parameters *params;
    PresentationTimer *timer;

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
    QHash<int, QString> annotations;

    QImage imgPreviousPage;
    QImage imgCurrentPage;
    QImage imgNextPage;

    void renderPreviousPage();
    void renderCurrentPage();
    void renderNextPage();

    void setPdfFileName(QString file);

public:
    explicit PDFModel(QObject *parent, Parameters *params, PresentationTimer *timer);
    ~PDFModel();
    bool pdfLoaded();
    void render();
    void gotoSpecificPage(int page);
    void gotoNextPage();
    void gotoPreviousPage();
    void gotoFirstPage();
    void gotoLastPage();
    void gotoOpenPage();

    int getFirstPage();
    int getPreviousPage();
    int getCurrentPage();
    int getNextPage();
    int getLastPage();

    QImage renderPdfPage(int page);
    QImage renderPdfPage(int page, QSizeF scaleFactor);

    QString getCurrentBeamerNote();
    void setCurrentBeamerNote(QString v);

    QImage& getImgPreviousPage();
    QImage& getImgCurrentPage();
    QImage& getImgNextPage();

    QSizeF getPageSize();
    QSizeF getScaleFactor();

    QString getPdfFileName(void);

signals:
    void renderingChanged(void);
    void presentationStarted(void);
    void presentationReset(void);

public slots:
    void handleModelSequence(QKeyEvent *e);

};

#endif // PDFMODEL_H

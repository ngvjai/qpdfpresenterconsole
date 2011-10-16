#ifndef PDFMODEL_H
#define PDFMODEL_H

#include <poppler-qt4.h>
#include <QObject>
#include <QtGui/QImage>
#include <QtGui/QDesktopWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QReadWriteLock>
#include "parameters.h"
#include "presentationtimer.h"
#include "app.h"

class PDFModel : public QObject
{
    Q_OBJECT
    Poppler::Document *document;
    Parameters *params;
    PresentationTimer *timer;

    int ContentPart;
    int AnnotationsPart;

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
    float scaleFactor;
    QHash<int, QString> annotations;
    QReadWriteLock mutexPreviousPage;
    QReadWriteLock mutexCurrentPage;
    QReadWriteLock mutexNextPage;

    QImage imgPreviousPage;
    QImage imgCurrentPage;
    QImage imgNextPage;

    void renderPreviousPage();
    void renderCurrentPage();
    void renderNextPage();

    void finishInit();
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

    int getContentPart();
    int getAnnotationsPart();
    int getFirstPage();
    int getPreviousPage();
    int getCurrentPage();
    int getNextPage();
    int getLastPage();

    QImage renderPdfPage(int page);
    QImage renderPdfPage(int page, QSizeF scaleFactor, int partie);

    QString getCurrentBeamerNote();
    void setBeamerNote(QString v, int page);

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
    void handleKeyModelSequence(QKeyEvent *e);
    void handleMouseModelSequence(QMouseEvent *e);
    void updateProjectorSize();

};

#endif // PDFMODEL_H

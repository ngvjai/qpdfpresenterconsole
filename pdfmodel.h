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
#include <QRegExp>
#include <QFile>
#include <QLabel>
#include "parameters.h"
#include "presentationtimer.h"
#include "textannot.h"
#include "app.h"
#include "mediaplayer.h"

class PDFModel : public QObject
{
    Q_OBJECT
    Poppler::Document *document;
    Parameters *params;
    PresentationTimer *timer;
    TextAnnot *textannot;

    QRegExp detectMediaFiles;

    QList<Poppler::Link*> gotoLinks;
    QList<Poppler::FileAttachmentAnnotation*> mediaFiles;
    QHash<int, int> nbMediaFilesInPage;
    QHash<QString, QByteArray> mediaContent;
    MediaPlayer* player;

    int ContentPart;
    int AnnotationsPart;

    int firstPage;
    int currentPage;
    int lastPage;
    int dpiX;
    int dpiY;
    int annotScale;
    QString pdfFileName;
    QRect projectorSize;
    QSizeF pageSize;
    float scaleFactorX;
    float scaleFactorY;
    float scaleFactor;
    QReadWriteLock mutexPreviousPage;
    QReadWriteLock mutexCurrentPage;
    QReadWriteLock mutexNextPage;
    QReadWriteLock mutexTextAnnot;

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
    QList<Poppler::Link*> getGotoLinks();
    QList<Poppler::FileAttachmentAnnotation*> getMediaFiles();
    QHash<QString, QByteArray> getMediaContent();
    QString getMediaTempFileName(Poppler::FileAttachmentAnnotation *fa);
    void createMediaPlayer(Poppler::FileAttachmentAnnotation *fa);
    void addMediaPlayerTarget(QWidget *widget);
    void pushTargetWidget(QWidget* widget);

    bool isMediaFile(Poppler::EmbeddedFile *file);
    bool hasMediaFile();
    void processCurrentPageAnnotations(Poppler::Page* page);
    QImage renderPdfPage(int page);
    QImage renderPdfPage(int page, QSizeF scaleFactor, int partie);

    QString getCurrentTextAnnot();
    void setTextAnnot(QString v, int page);

    QImage& getImgPreviousPage();
    QImage& getImgCurrentPage();
    QImage& getImgNextPage();

    QSizeF getPageSize();
    QSizeF getScaleFactor();
    float getDpiX();
    float getDpiY();
    int getAnnotScale();

    QString& getPdfFileName(void);

signals:
    void renderingChanged(void);
    void presentationStarted(void);
    void presentationReset(void);
    void mediaFilesReady(void);

public slots:
    void handleKeyModelSequence(QKeyEvent *e);
    void handleMouseModelSequence(QMouseEvent *e);
    void handleMouseWheelModelSequence(QWheelEvent *e);
    void updateProjectorSize();
    void updateTextAnnot();
    void startMediaPlayer();
    void stopMediaPlayer();
    void pauseMediaPlayer();
    void gotoNextPage();
    void gotoPreviousPage();

};

#endif // PDFMODEL_H

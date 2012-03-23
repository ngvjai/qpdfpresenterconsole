/* vim: set et ts=4 sw=4: */

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
#include <QTemporaryFile>
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
    QList<QString> filesToDelete;
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
    void autoDetectBeamerNotes();

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
    void addMediaPlayerTarget(QWidget *widget, bool toMute = false);
    void pushTargetWidget(QWidget* widget);

    bool isMediaFile(Poppler::EmbeddedFile *file);
    bool isMediaFile(Poppler::FileAttachmentAnnotation *fa);
    bool hasMediaFile();
    int getMediaPlayerVolume();
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
    void setPdfFileName(QString file);

signals:
    void pdfFileNameReady(void);
    void renderingChanged(void);
    void presentationStarted(void);
    void presentationReset(void);
    void mediaFilesReady(void);

    void notifyWorkStarted(void);
    void notifyWorkFinished(void);

    void mediaTimeChanged(qint64 milisecpos);
    void mediaPositionChanged(float position);

public slots:
    void pdfFileNameSet(void);

    void handleKeyModelSequence(QKeyEvent *e);
    void handleMouseModelSequence(QMouseEvent *e);
    void handleMouseWheelModelSequence(QWheelEvent *e);

    void updateProjectorSize();
    void updateTextAnnot();
    void gotoNextPage();
    void gotoPreviousPage();

    void startMediaPlayer();
    void stopMediaPlayer();
    void pauseMediaPlayer();
    void seekMediaPlayer(float position);
    void setMediaPlayerVolume(int volume);
    void getNewMediaTime(qint64 milisecpos);
    void getNewMediaPosition(float position);

    void setApplicationCursorWorking(void);
    void setApplicationCursorIdling(void);
};

#endif // PDFMODEL_H

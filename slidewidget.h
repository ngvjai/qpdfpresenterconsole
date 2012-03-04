#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>
#include <QPainter>
#include <QFile>
#include <QDir>
#include "pdfmodel.h"
#include "parameters.h"
#include "mousemoveeventeater.h"
#include "mediaplayer.h"

class SlideWidget : public QLabel
{
    Q_OBJECT
    PDFModel *modele;
    QWidget *parent;
    Parameters *params;
    QLabel *video;
    MouseMoveEventEater *mmee;
    bool mediaFilesReady;

    QPointF computeScaledPos(QPoint pos);
    QPointF computeScaledPdfPos(QPointF pos);
    QRectF scalePdfArea(QRectF area);

public:
    explicit SlideWidget(QWidget *parent = 0, PDFModel *modele = 0, Parameters *params = 0);
    ~SlideWidget();
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    QSize getDeltaToAdd();
    QRectF getContentRect(int margin = 0);

protected:
    void resizeEvent(QResizeEvent *ev);
    void paintEvent(QPaintEvent *ev);

signals:

public slots:
    void updateView();

};

#endif // SLIDEWIDGET_H

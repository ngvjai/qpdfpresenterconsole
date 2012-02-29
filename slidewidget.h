#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>
#include <QPainter>
#include "mainscreenpdfview.h"
#include "pdfmodel.h"

class SlideWidget : public QLabel
{
    Q_OBJECT
    PDFModel *modele;
    MainScreenPdfView *parent;

    QPointF computeScaledPos(QPoint pos);

public:
    explicit SlideWidget(MainScreenPdfView *parent = 0, PDFModel *modele = 0);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void paintEvent(QPaintEvent *ev);

signals:

public slots:

};

#endif // SLIDEWIDGET_H

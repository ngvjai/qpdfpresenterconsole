#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>
#include <QPainter>
#include <QFile>
#include <QDir>
#include "pdfmodel.h"
#include "mousemoveeventeater.h"
#include "mediaplayer.h"

class SlideWidget : public QLabel
{
    Q_OBJECT
    PDFModel *modele;
    QWidget *parent;
    QLabel *video;
    MouseMoveEventEater *mmee;

    QPointF computeScaledPos(QPoint pos);

public:
    explicit SlideWidget(QWidget *parent = 0, PDFModel *modele = 0);
    ~SlideWidget();
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:

public slots:
    void updateView();

};

#endif // SLIDEWIDGET_H

#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>
#include "pdfmodel.h"

class SlideWidget : public QLabel
{
    Q_OBJECT
    PDFModel *modele;

    QPointF computeScaledPos(QPoint pos);

public:
    explicit SlideWidget(QWidget *parent = 0, PDFModel *modele = 0);
    void mouseMoveEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);

signals:

public slots:

};

#endif // SLIDEWIDGET_H

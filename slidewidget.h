#ifndef SLIDEWIDGET_H
#define SLIDEWIDGET_H

#include <QLabel>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <vlc/vlc.h>
#include "pdfmodel.h"

class SlideWidget : public QLabel
{
    Q_OBJECT
    PDFModel *modele;
    QWidget *parent;
    QLabel *video;

    bool vlc_playing;
    libvlc_instance_t *vlc_instance;
    libvlc_media_player_t *vlc_media_player;
    libvlc_media_t *vlc_media;

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

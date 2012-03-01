#include "slidewidget.h"

SlideWidget::SlideWidget(QWidget *parent, PDFModel *modele) :
    QLabel(parent)
{
    this->parent = parent;
    this->modele = modele;
    this->video = new QLabel(this);
    this->video->hide();
    this->setMouseTracking(true);
    this->video->setMouseTracking(true);

    this->mmee = new MouseMoveEventEater(this);

    this->video->installEventFilter(mmee);

    this->vlc_playing = false;
    this->vlc_instance = libvlc_new(0, NULL);

    QObject::connect(this->modele, SIGNAL(mediaFilesReady()), SLOT(updateView()));
}

SlideWidget::~SlideWidget()
{
    libvlc_release(this->vlc_instance);
}

QPointF SlideWidget::computeScaledPos(QPoint pos)
{
    return QPointF(
                (pos.x() / (1.0 * this->width())),
                (pos.y() / (1.0 * this->height()))
    );
}

void SlideWidget::mouseMoveEvent(QMouseEvent * ev)
{
    ev->accept();
    QPointF scaledPos = this->computeScaledPos(ev->pos());

    this->setCursor(Qt::ArrowCursor);
    this->setToolTip("");

    /* Check for links */
    foreach(Poppler::Link* link, this->modele->getGotoLinks()) {
        if (link->linkArea().contains(scaledPos)) {
            this->setCursor(Qt::PointingHandCursor);
            break;
        }
    }

    /* Check for multimedia stuff */
    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
        if (this->modele->isMediaFile(fa->embeddedFile()) && fa->boundary().contains(scaledPos)) {
            this->setCursor(Qt::PointingHandCursor);
            this->setToolTip(fa->embeddedFile()->name());
            break;
        }
    }
}

void SlideWidget::mouseReleaseEvent(QMouseEvent *ev)
{
    ev->accept();
    QPointF scaledPos = this->computeScaledPos(ev->pos());

    foreach(Poppler::Link* link, this->modele->getGotoLinks()) {
        if (link->linkArea().contains(scaledPos)) {
            const Poppler::LinkGoto *gotoLink = dynamic_cast<const Poppler::LinkGoto*>(link);
            Poppler::LinkDestination dest = gotoLink->destination();
            /* pageNumber() is [1;...] */
            this->modele->gotoSpecificPage(dest.pageNumber() - 1);
            break;
        }
    }

    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
        if (this->modele->isMediaFile(fa->embeddedFile()) && fa->boundary().contains(scaledPos)) {
            QString fname = QDir::tempPath() + QDir::separator() + appname + "_" + fa->embeddedFile()->name();
            if (this->vlc_playing) {
                this->vlc_playing = false;
                libvlc_media_player_stop(this->vlc_media_player);
                libvlc_media_player_release(this->vlc_media_player);
                libvlc_release(this->vlc_instance);
                QFile::remove(fname);
            } else {
                QByteArray data(this->modele->getMediaContent().value(fa->embeddedFile()->name()));
                QFile media(fname);
                if (media.open(QIODevice::WriteOnly)) {
                    media.write(data);
                    media.close();

                    this->vlc_media = libvlc_media_new_path(this->vlc_instance, fname.toLocal8Bit().data());
                    this->vlc_media_player = libvlc_media_player_new_from_media(this->vlc_media);
                    libvlc_media_release(this->vlc_media);
#ifdef Q_WS_WIN
                    libvlc_media_player_set_drawable(
                                this->vlc_media_player,
                                reinterpret_cast<unsigned int>(this->video->winId()));
#endif
#ifdef Q_WS_MAC
                    libvlc_media_player_set_drawable(
                                this->vlc_media_player,
                                this->video->winId());
#endif
#ifdef Q_WS_X11
                    libvlc_media_player_set_xwindow(
                                this->vlc_media_player,
                                this->video->winId());
#endif
                    libvlc_media_player_play (this->vlc_media_player);

                    this->vlc_playing = true;
                }
            }
            break;
        }
    }
}

void SlideWidget::updateView()
{
    if (!this->modele->hasMediaFile()) {
        this->video->hide();
        return;
    }

    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
        QPointF b1(fa->boundary().left() * this->width(), fa->boundary().top() * this->height());
        QPointF b2(fa->boundary().right() * this->width(), fa->boundary().bottom() * this->height());

        QRectF bounds(b1, b2);

        this->video->move(b1.toPoint());
        this->video->resize(bounds.width(), bounds.height());
    }

    this->video->show();
}

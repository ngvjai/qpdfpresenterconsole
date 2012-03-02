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

    QObject::connect(this->modele, SIGNAL(mediaFilesReady()), SLOT(updateView()));
}

SlideWidget::~SlideWidget()
{
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
            this->modele->startMediaPlayer();
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

    this->modele->addMediaPlayerTarget(this->video);
    this->video->show();
}

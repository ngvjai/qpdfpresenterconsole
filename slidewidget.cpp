#include "slidewidget.h"
#include <iostream>

SlideWidget::SlideWidget(MainScreenPdfView *parent, PDFModel *modele) :
    QLabel(parent)
{
    this->parent = parent;
    this->modele = modele;
    this->setMouseTracking(true);
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
    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getVideos()) {
        if (this->modele->isMediaFile(fa->embeddedFile()) && fa->boundary().contains(scaledPos)) {
            this->setCursor(Qt::PointingHandCursor);
            this->setToolTip(fa->embeddedFile()->name());
            break;
        }
    }
}

void SlideWidget::mouseReleaseEvent(QMouseEvent *ev)
{
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
}

void SlideWidget::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);
    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getVideos()) {
        QRectF bounds(
            QPointF(
                        fa->boundary().left() * this->width(),
                        fa->boundary().top() * this->height()
            ),
            QPointF(
                        fa->boundary().right() * this->width(),
                        fa->boundary().bottom() * this->height()
            )
        );

        Poppler::EmbeddedFile *ef = fa->embeddedFile();

        QPainter painter(this);
        painter.setPen(Qt::blue);
        painter.drawRect(bounds);
    }
}

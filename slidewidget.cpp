#include "slidewidget.h"
#include <iostream>

SlideWidget::SlideWidget(QWidget *parent, PDFModel *modele) :
    QLabel(parent)
{
    this->modele = modele;
    this->setMouseTracking(true);
}

QPointF SlideWidget::computeScaledPos(QPoint pos)
{
    return QPointF(
                (pos.x() / (1.0 * this->width())),
                (pos.y() / (1.0 * this->height()))
            // (pos.x() - (this->width() - this->currentSlide->pixmap()->width())) / this->modele->getScaleFactor().width() / physicalDpiX() * this->modele->getDpiX() / this->modele->getPageSize().width(),
            // (pos.y() - (this->height() - this->currentSlide->pixmap()->height())) / this->modele->getScaleFactor().height() / physicalDpiY() * this->modele->getDpiY() / this->modele->getPageSize().height()
    );
}

void SlideWidget::mouseMoveEvent(QMouseEvent * ev)
{
    QPointF scaledPos = this->computeScaledPos(ev->pos());

    this->setCursor(Qt::ArrowCursor);

    foreach(Poppler::Link* link, this->modele->getGotoLinks()) {
/*
        std::cerr << "(topLeft:" << link->linkArea().topLeft().x() << ":" << link->linkArea().topLeft().y();
        std::cerr << "; topRight:" << link->linkArea().topRight().x() << ":" << link->linkArea().topRight().y();
        std::cerr << "; bottomLeft:" << link->linkArea().bottomLeft().x() << ":" << link->linkArea().bottomLeft().y();
        std::cerr << "; bottomRight:" << link->linkArea().bottomRight().x() << ":" << link->linkArea().bottomRight().y();
        std::cerr << " || (" << scaledPos.x() << ";" << scaledPos.y() << ")" << std::endl;
*/
        if (link->linkArea().contains(scaledPos)) {
            this->setCursor(Qt::PointingHandCursor);
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

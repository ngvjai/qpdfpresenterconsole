#include "slidewidget.h"
#include <iostream>

SlideWidget::SlideWidget(QWidget *parent, PDFModel *modele, Parameters *params) :
    QLabel(parent)
{
    this->parent = parent;
    this->modele = modele;
    this->params = params;

    this->mediaFilesReady = false;
    this->video = new QLabel(this);
    this->video->hide();
    this->setMouseTracking(true);
    this->video->setMouseTracking(true);

    this->mmee = new MouseMoveEventEater(this);

    this->video->installEventFilter(mmee);

    QObject::connect(this->modele, SIGNAL(mediaFilesReady()), SLOT(updateView()));
    QObject::connect(this, SIGNAL(mouseWheelEvent(QWheelEvent*)),
                     this->modele, SLOT(handleMouseWheelModelSequence(QWheelEvent*)));
}

SlideWidget::~SlideWidget()
{
}

void SlideWidget::resizeEvent(QResizeEvent *ev)
{
    ev->accept();
    this->updateView();
}

QSize SlideWidget::getDeltaToAdd()
{
    return QSize(
        (this->contentsRect().width() - this->pixmap()->rect().width()) / 2,
        (this->contentsRect().height() - this->pixmap()->rect().height()) / 2
    );
}

QRectF SlideWidget::getContentRect(int margin)
{
    QSize deltaToAdd = this->getDeltaToAdd();

    return QRectF(
        QPointF(
            this->pixmap()->rect().x() + deltaToAdd.width() + margin,
            this->pixmap()->rect().y() + deltaToAdd.height() + margin),
        QPointF(
            this->pixmap()->rect().width() + deltaToAdd.width() - margin,
            this->pixmap()->rect().height() + deltaToAdd.height() - margin)
    );
}

QPointF SlideWidget::computeScaledPos(QPoint pos)
{
    /* QRectF cRect = this->getContentRect();

    return QPointF(
                pos.x() * (cRect.width() * this->modele->getAnnotScale()),
                pos.y() * cRect.height()
    );
    */
    return QPointF(pos);
}

QPointF SlideWidget::computeScaledPdfPos(QPointF pos)
{
    QRectF cRect = this->getContentRect();
    QSize delta = this->getDeltaToAdd();

    return QPointF(
                delta.width() + pos.x() * cRect.width() * this->modele->getAnnotScale(),
                delta.height() + pos.y() * cRect.height()
                );
}

QRectF SlideWidget::scalePdfArea(QRectF area)
{
    return QRectF(
                this->computeScaledPdfPos(area.topLeft()),
                this->computeScaledPdfPos(area.bottomRight())
                );
}

void SlideWidget::paintEvent(QPaintEvent *ev)
{
    ev->accept();
    QLabel::paintEvent(ev);

#ifdef HAVE_DEBUG_AREA
    QPainter p(this);

    p.setPen(Qt::blue);
    p.drawRect(this->getContentRect(5));

    if (this->parent->windowTitle() == "MainScreenPdfView") {
        p.setPen(Qt::red);
    }
    if (this->parent->windowTitle() == "PresenterPdf") {
        p.setPen(Qt::green);
    }

    foreach(Poppler::Link* link, this->modele->getGotoLinks()) {
        QRectF scaledArea = this->scalePdfArea(link->linkArea());
        p.drawRect(scaledArea);
    }

    if (this->parent->windowTitle() == "MainScreenPdfView") {
        p.setPen(Qt::darkRed);
    }
    if (this->parent->windowTitle() == "PresenterPdf") {
        p.setPen(Qt::darkGreen);
    }

    if (this->modele->hasMediaFile()) {
        foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
            QRectF scaledArea = this->scalePdfArea(fa->boundary());
            p.drawRect(scaledArea);
        }
    }

    p.end();
#endif
}

void SlideWidget::mouseMoveEvent(QMouseEvent * ev)
{
    ev->accept();
    QPointF scaledPos = this->computeScaledPos(ev->pos());

    this->setCursor(Qt::ArrowCursor);
    this->setToolTip("");

    /* Check for links */
    foreach(Poppler::Link* link, this->modele->getGotoLinks()) {
        QRectF scaledArea = this->scalePdfArea(link->linkArea());
        if (scaledArea.contains(scaledPos)) {
            this->setCursor(Qt::PointingHandCursor);
            break;
        }
    }

    /* Check for multimedia stuff */
    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
        QRectF scaledArea = this->scalePdfArea(fa->boundary());
        if (this->modele->isMediaFile(fa->embeddedFile()) && scaledArea.contains(scaledPos)) {
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
        QRectF scaledArea = this->scalePdfArea(link->linkArea());
        if (scaledArea.contains(scaledPos)) {
            const Poppler::LinkGoto *gotoLink = dynamic_cast<const Poppler::LinkGoto*>(link);
            Poppler::LinkDestination dest = gotoLink->destination();
            /* pageNumber() is [1;...] */
            this->modele->gotoSpecificPage(dest.pageNumber() - 1);
            break;
        }
    }

    foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
        QRectF scaledArea = this->scalePdfArea(fa->boundary());
        if (this->modele->isMediaFile(fa->embeddedFile()) && scaledArea.contains(scaledPos)) {
            this->modele->startMediaPlayer();
            break;
        }
    }
}

void SlideWidget::wheelEvent(QWheelEvent *ev)
{
    emit mouseWheelEvent(ev);
}

void SlideWidget::updateView()
{
    if (!this->modele->hasMediaFile()) {
        this->video->hide();
        return;
    }

    if (this->parent->windowTitle() == "MainScreenPdfView" && this->params->getBeamerNotes()) {
        this->video->hide();
        return;
    }

    // foreach(Poppler::FileAttachmentAnnotation *fa, this->modele->getMediaFiles()) {
    Poppler::FileAttachmentAnnotation *fa = this->modele->getMediaFiles().first();
        QRectF scaledArea = this->scalePdfArea(fa->boundary());
        this->video->move(scaledArea.topLeft().toPoint());
        this->video->resize(scaledArea.size().toSize());
    // }

    this->modele->addMediaPlayerTarget(this->video);
    this->video->show();
}

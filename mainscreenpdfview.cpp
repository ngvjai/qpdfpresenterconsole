#include "mainscreenpdfview.h"
#include <QtGui/QVBoxLayout>

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele) :
    QMainWindow(parent)
{
    QVBoxLayout *vlayout = new QVBoxLayout(this);
    QHBoxLayout *hlayout = new QHBoxLayout(this);
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->currentSlide = new QLabel(this);
    this->nextSlide = new QLabel(this);

    vlayout->addWidget(this->currentSlide);
    vlayout->addWidget(this->nextSlide);
    vlayout->addLayout(hlayout);
    vlayout->addWidget(this->slides);

    fake->setLayout(vlayout);
    this->setCentralWidget(fake);

    this->modele = modele;
    QObject::connect(modele, SIGNAL(renderingChanged()), SLOT(updateView()));
}

void MainScreenPdfView::keyReleaseEvent(QKeyEvent * ev)
{
    if (ev->isAutoRepeat()) {
        ev->ignore();
    } else {
        switch(ev->key())
        {
        case Qt::Key_Escape:
            exit(EXIT_SUCCESS);
            break;

        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_PageUp:
            modele->gotoPreviousPage();
            break;

        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Enter:
        case Qt::Key_PageDown:
            modele->gotoNextPage();
            break;

        case Qt::Key_Home:
            modele->gotoFirstPage();
            break;

        case Qt::Key_End:
            modele->gotoLastPage();
            break;

        default:
            break;
        }
    }
}

void MainScreenPdfView::updateView()
{
    QString slides = "%1/%2";
    this->slides->setText(
            slides
            .arg(this->modele->getCurrentPage() + 1)
            .arg(this->modele->getLastPage() + 1)
            );

    QSize currentSize(this->modele->getPageSize().height()*1.75, this->modele->getPageSize().width()*1.75);
    QSize nextSize(this->modele->getPageSize().height()*1.35, this->modele->getPageSize().width()*1.35);

    QImage currentScaled = QImage(this->modele->getImgCurrentPage())
                           .scaled(currentSize,
                                   Qt::IgnoreAspectRatio,
                                   Qt::FastTransformation);
    QImage nextScaled = QImage(this->modele->getImgNextPage())
                        .scaled(nextSize,
                                Qt::IgnoreAspectRatio,
                                Qt::FastTransformation);

    this->currentSlide->setPixmap(QPixmap::fromImage(currentScaled));
    this->nextSlide->setPixmap(QPixmap::fromImage(nextScaled));
}

#include "mainscreenpdfview.h"
#include <QtGui/QGridLayout>
#include <QApplication>

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele, Parameters *params, PresentationTimer *timer) :
    QMainWindow(parent)
{
    QGridLayout *glayout = new QGridLayout(this);
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->timer = new QLabel(this);
    this->currentSlide = new QLabel(this);
    this->nextSlide = new QLabel(this);
    this->timer->setStyleSheet(
            this->timer->styleSheet()
            .append("color: white;"));
    this->timer->setStyleSheet(
            this->timer->styleSheet()
            .append("font-weight: bold;"));
    this->slides->setStyleSheet(
            this->slides->styleSheet()
            .append("color: white; font-weight: bold;"));

    /* Setting fontSize */
    QFont slidesFont(this->slides->font());
    QFont timerFont(this->timer->font());
    slidesFont.setPointSize(32);
    timerFont.setPointSize(32);

    this->slides->setFont(slidesFont);
    this->timer->setFont(timerFont);

    this->setStyleSheet("background-color: black;");

    glayout->addWidget(this->currentSlide, 0, 0, Qt::AlignCenter);
    glayout->addWidget(this->nextSlide, 0, 1, Qt::AlignCenter);
    glayout->addWidget(this->slides, 2, 0, Qt::AlignCenter);
    glayout->addWidget(this->timer, 2, 1, Qt::AlignCenter);

    fake->setLayout(glayout);
    this->setCentralWidget(fake);

    this->modele = modele;
    this->params = params;
    this->pTimer = timer;

    this->timerUpdated();

    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
    QObject::connect(this->pTimer, SIGNAL(timerChanged()), SLOT(timerUpdated()));
    QObject::connect(this, SIGNAL(keyPressed(QKeyEvent*)),
                     this->modele, SLOT(handleModelSequence(QKeyEvent*)));
}

void MainScreenPdfView::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat()) {
        ev->ignore();
    } else {
        switch(ev->key())
        {
        case Qt::Key_Escape:
            QCoreApplication::quit();
            break;

        case Qt::Key_Space:
            this->pTimer->launch();
            break;

        default:
            emit keyPressed(ev);
            break;
        }
    }
}

void MainScreenPdfView::updateView()
{
    this->slides->setText(
            QObject::tr("Slide %1 of %2")
                    .arg(this->modele->getCurrentPage() + 1)
                    .arg(this->modele->getLastPage() + 1)
            );

    float f1 = (QApplication::desktop()->screenGeometry(this).width() * this->params->getCurrentSlidePrcentWidth() - 15) / this->modele->getPageSize().width();
    float f2 = (QApplication::desktop()->screenGeometry(this).width() * (1 - this->params->getCurrentSlidePrcentWidth()) - 15) / this->modele->getPageSize().width();

    QImage currentScaled = QImage(
            this->modele->renderPdfPage(
                    this->modele->getCurrentPage(),
                    QSizeF(f1, f1)
                    )
            );

    QImage nextScaled = QImage(
            this->modele->renderPdfPage(
                    this->modele->getNextPage(),
                    QSizeF(f2, f2)
                    )
            );

    this->currentSlide->setPixmap(QPixmap::fromImage(currentScaled));
    this->nextSlide->setPixmap(QPixmap::fromImage(nextScaled));
}

void MainScreenPdfView::timerUpdated()
{
    QTime temps(this->pTimer->getPresentationHours(), this->pTimer->getPresentationMinutes(), this->pTimer->getPresentationSeconds(), 0);
    QString s_temps(temps.toString("hh:mm:ss"));
    this->timer->setText(s_temps);

    if (this->pTimer->isCritical()) {
        this->timer->setStyleSheet(this->timer->styleSheet().append("color: red;"));
    }
}

#include "mainscreenpdfview.h"
#include <QtGui/QGridLayout>
#include <QApplication>
#include <stdio.h>

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele, Parameters *params, PresentationTimer *timer) :
    QMainWindow(parent)
{
    QRect res = QApplication::desktop()->availableGeometry(params->getMainScreenId());
    this->move(res.x(), res.y());
    this->showFullScreen();

    QGridLayout *glayout = new QGridLayout(this);
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->timer = new QLabel(this);
    this->currentSlide = new QLabel(this);
    this->nextSlide = new QLabel(this);
    this->currentDate = new QLabel(this);
    this->emergencyDate = new QLabel(this);

    this->timer->setStyleSheet(
            this->timer->styleSheet()
            .append("color: white;"));
    this->timer->setStyleSheet(
            this->timer->styleSheet()
            .append("font-weight: bold;"));
    this->slides->setStyleSheet(
            this->slides->styleSheet()
            .append("color: white; font-weight: bold;"));
    this->emergencyDate->setStyleSheet(
            this->emergencyDate->styleSheet()
            .append("color: red; font-weight: bold;")
            );
    this->currentDate->setStyleSheet(
            this->currentDate->styleSheet()
            .append("color: white; font-weight: bold;")
            );

    /* Setting fontSize */
    QFont slidesFont(this->slides->font());
    QFont timerFont(this->timer->font());
    QFont dateFont(this->currentDate->font());
    QFont emergencyFont(this->emergencyDate->font());
    slidesFont.setPointSize(32);
    timerFont.setPointSize(32);
    dateFont.setPointSize(32);
    emergencyFont.setPointSize(32);

    this->slides->setFont(slidesFont);
    this->timer->setFont(timerFont);
    this->currentDate->setFont(dateFont);
    this->emergencyDate->setFont(emergencyFont);

    this->setStyleSheet("background-color: black;");

    glayout->addWidget(this->currentDate, 0, 0, Qt::AlignCenter);
    glayout->addWidget(this->emergencyDate, 0, 1, Qt::AlignCenter);
    glayout->addWidget(this->currentSlide, 1, 0, Qt::AlignCenter);
    glayout->addWidget(this->nextSlide, 1, 1, Qt::AlignCenter);
    glayout->addWidget(this->slides, 2, 0, Qt::AlignCenter);
    glayout->addWidget(this->timer, 2, 1, Qt::AlignCenter);

    fake->setLayout(glayout);
    this->setCentralWidget(fake);

    this->modele = modele;
    this->params = params;
    this->pTimer = timer;

    this->timerUpdated();
    /* Timer for current date displaying */
    this->startTimer(1000);

    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
    QObject::connect(this->pTimer, SIGNAL(timerChanged()), SLOT(timerUpdated()));
    QObject::connect(this, SIGNAL(keyPressed(QKeyEvent*)),
                     this->modele, SLOT(handleModelSequence(QKeyEvent*)));
    QObject::connect(this, SIGNAL(presentationStarted()),
                     this->pTimer, SLOT(startCounterIfNeeded()));
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

        case Qt::Key_F:
            if (this->maximized) {
                this->showNormal();
            } else {
                this->showFullScreen();
            }
            break;

        case Qt::Key_Space:
            emit presentationStarted();
            break;

        default:
            emit keyPressed(ev);
            break;
        }
    }
}

void MainScreenPdfView::resizeEvent(QResizeEvent *ev)
{
    QRect desktop = QApplication::desktop()->screenGeometry(this);
    QSize dSize(desktop.width(), desktop.height());
    QSize wSize = ev->size();

    this->maximized = (dSize == wSize);
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

    this->emergencyDate->setText(
            QString(
                    QTime(
                            this->pTimer->getEmergencyHours(),
                            this->pTimer->getEmergencyMinutes(),
                            this->pTimer->getEmergencySeconds(), 0
                            )
                    .toString("hh:mm:ss")
                    )
            );
}

void MainScreenPdfView::timerUpdated()
{
    this->timer->setText(
            QString(
                    QTime(
                            this->pTimer->getPresentationHours(),
                            this->pTimer->getPresentationMinutes(),
                            this->pTimer->getPresentationSeconds(), 0
                            )
                    .toString("hh:mm:ss")
                    )
            );

    if (this->pTimer->isCritical()) {
        this->timer->setStyleSheet(this->timer->styleSheet().append("color: red;"));
    }
}

void MainScreenPdfView::timerEvent(QTimerEvent *ev)
{
    this->currentDate->setText(QDateTime::currentDateTime().toString(Qt::DefaultLocaleLongDate));
}

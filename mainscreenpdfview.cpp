#include "mainscreenpdfview.h"
#include <QtGui/QGridLayout>
#include <QApplication>
#include <QInputDialog>
#include "optionsdialog.h"

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele, Parameters *params, PresentationTimer *timer) :
    QMainWindow(parent)
{
    this->modele = modele;
    this->params = params;
    this->pTimer = timer;
    this->options = NULL;

    QGridLayout *glayout = new QGridLayout();
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->timer = new QLabel(this);
    this->currentSlide = new QLabel(this);
    this->nextSlide = new QLabel(this);
    this->currentDate = new QLabel(this);
    this->emergencyDate = new QLabel(this);
    this->beamerNote = new QLabel(this);

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
    QFont beamerNoteFont(this->beamerNote->font());
    slidesFont.setPointSize(24);
    timerFont.setPointSize(24);
    dateFont.setPointSize(24);
    emergencyFont.setPointSize(24);
    beamerNoteFont.setPointSize(16);

    this->slides->setFont(slidesFont);
    this->timer->setFont(timerFont);
    this->currentDate->setFont(dateFont);
    this->emergencyDate->setFont(emergencyFont);
    this->beamerNote->setFont(beamerNoteFont);

    glayout->addWidget(this->currentDate,   0, 0, Qt::AlignCenter);
    glayout->addWidget(this->emergencyDate, 0, 1, Qt::AlignCenter);
    glayout->addWidget(this->currentSlide,  1, 0, Qt::AlignCenter);
    glayout->addWidget(this->nextSlide,     1, 1, Qt::AlignCenter);
    glayout->addWidget(this->beamerNote,    2, 0, 1, -1, Qt::AlignCenter);
    glayout->addWidget(this->slides,        3, 0, Qt::AlignCenter);
    glayout->addWidget(this->timer,         3, 1, Qt::AlignCenter);

    fake->setLayout(glayout);
    this->setCentralWidget(fake);
    this->setStyleSheet("background-color: black;");

    this->beamerNote->setWordWrap(true);
    this->beamerNote->setStyleSheet(
            this->beamerNote->styleSheet()
            .append("color: white;")
            );

    this->timerUpdated();
    /* Timer for current date displaying */
    this->startTimer(1000);

    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
    QObject::connect(this->pTimer, SIGNAL(timerChanged()), SLOT(timerUpdated()));
    QObject::connect(this, SIGNAL(keyPressed(QKeyEvent*)),
                     this->modele, SLOT(handleKeyModelSequence(QKeyEvent*)));
    QObject::connect(this, SIGNAL(mousePressed(QMouseEvent*)),
                     this->modele, SLOT(handleMouseModelSequence(QMouseEvent*)));
    QObject::connect(this, SIGNAL(presentationStarted()),
                     this->pTimer, SLOT(startCounterIfNeeded()));
    QObject::connect(this->params, SIGNAL(mainScreenChanged()), SLOT(moveToScreen()));

    this->moveToScreen();
}

void MainScreenPdfView::moveToScreen()
{
    this->showNormal();
    QRect res = QApplication::desktop()->screenGeometry(this->params->getMainScreenId());
    this->move(res.x(), res.y());
    this->showFullScreen();

    this->f1 = (QApplication::desktop()->screenGeometry(this).width() * this->params->getCurrentSlidePrcentWidth() - 15) / this->modele->getPageSize().width();
    this->f2 = (QApplication::desktop()->screenGeometry(this).width() * (1 - this->params->getCurrentSlidePrcentWidth()) - 15) / this->modele->getPageSize().width();

    this->updateView();
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

        case Qt::Key_G:
            {
                bool ok;
                int p = QInputDialog::getInt(0,
                                         QObject::tr("Going to a specific page number"),
                                         QObject::tr("Going to page:"),
                                         this->modele->getCurrentPage() + 1,
                                         this->modele->getFirstPage() + 1,
                                         this->modele->getLastPage() + 1,
                                         1, &ok);
                if (ok) {
                    this->modele->gotoSpecificPage(p - 1);
                }
            }
            break;

        case Qt::Key_O:
            if (!this->options) {
                this->options = new OptionsDialog(0, this->params);
                QObject::connect(this->options, SIGNAL(paramsChanged()), SLOT(updateView()));
                QObject::connect(this->options, SIGNAL(paramsChanged()), SLOT(timerUpdated()));
                QObject::connect(this->options, SIGNAL(resetPresentationCounter()), SLOT(resetPresentationTimer()));
            }
            this->options->show();
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

void MainScreenPdfView::mouseReleaseEvent(QMouseEvent *ev)
{
    switch(ev->button()) {
    default:
        emit mousePressed(ev);
        break;
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

    if (!this->params->getBeamerNotes()) {
        // If we don't have beamer notes, display current and next slide content
        this->currentSlide->setPixmap(
                QPixmap::fromImage(
                        this->modele->renderPdfPage(
                                this->modele->getCurrentPage(),
                                QSizeF(this->f1, this->f1),
                                this->modele->getContentPart()
                                )
                        )
                );

        this->nextSlide->setPixmap(
                QPixmap::fromImage(
                        this->modele->renderPdfPage(
                                this->modele->getNextPage(),
                                QSizeF(this->f2, this->f2),
                                this->modele->getContentPart()
                                )
                        )
                );
    } else {
        // If we have beamer notes, display current slide content and its notes
        this->currentSlide->setPixmap(
                QPixmap::fromImage(
                        this->modele->renderPdfPage(
                                this->modele->getCurrentPage(),
                                QSizeF(this->f1, this->f1),
                                this->modele->getAnnotationsPart()
                                )
                        )
                );

        this->nextSlide->setPixmap(
                QPixmap::fromImage(
                        this->modele->renderPdfPage(
                                this->modele->getCurrentPage(),
                                QSizeF(this->f2, this->f2),
                                this->modele->getContentPart()
                                )
                        )
                );
    }

    this->beamerNote->setFixedWidth(this->currentSlide->width() + this->nextSlide->width());
    this->beamerNote->setText(this->modele->getCurrentTextAnnot());
}

void MainScreenPdfView::resetPresentationTimer()
{
    this->pTimer->resetCounter();
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
    this->currentDate->setText(
            QDateTime::currentDateTime()
            .toString(
                    Qt::DefaultLocaleShortDate
                    )
            );
}

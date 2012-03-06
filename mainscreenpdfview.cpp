#include "mainscreenpdfview.h"
#include <QtGui/QGridLayout>
#include <QApplication>
#include <QInputDialog>
#include <QStyle>
#include "optionsdialog.h"
#include "slidewidget.h"
#include <iostream>

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele, Parameters *params, PresentationTimer *timer, ScreenSaverInhibit *screensaverinhibiter) :
    QMainWindow(parent)
{
    this->modele = modele;
    this->params = params;
    this->pTimer = timer;
    this->options = NULL;
    this->screensaver = screensaverinhibiter;

    this->setWindowTitle("MainScreenPdfView");

    QGridLayout *glayout = new QGridLayout();
    QHBoxLayout *medialayout = new QHBoxLayout();
    QHBoxLayout *barlayout = new QHBoxLayout();
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->timer = new QLabel(this);
    this->currentSlide = new SlideWidget(this, modele, params);
    this->nextSlide = new QLabel(this);
    this->currentDate = new QLabel(this);
    this->emergencyDate = new QLabel(this);
    this->beamerNote = new QLabel(this);
    this->mediabar = new QToolBar(this);
    this->controlbar = new QToolBar(this);

    /* media playback stuff */
    this->playAction = new QAction(this->style()->standardIcon(QStyle::SP_MediaPlay), tr("Play"), this);
    this->pauseAction = new QAction(this->style()->standardIcon(QStyle::SP_MediaPause), tr("Pause"), this);
    this->stopAction = new QAction(this->style()->standardIcon(QStyle::SP_MediaStop), tr("Stop"), this);
    // this->previousAction = new QAction(this->style()->standardIcon(QStyle::SP_MediaSkipBackward), tr("Previous"), this);
    // this->nextAction = new QAction(this->style()->standardIcon(QStyle::SP_MediaSkipForward), tr("Next"), this);
    this->playAction->setShortcut(tr("Ctrl+P"));
    this->pauseAction->setShortcut(tr("Ctrl+A"));
    this->stopAction->setShortcut(tr("Ctrl+S"));
    // this->nextAction->setShortcut(tr("Ctrl+N"));
    // this->previousAction->setShortcut(tr("Ctrl+R"));
    this->mediabar->addAction(this->playAction);
    this->mediabar->addAction(this->pauseAction);
    this->mediabar->addAction(this->stopAction);
    // this->mediabar->addAction(this->previousAction);
    // this->mediabar->addAction(this->nextAction);
    this->mediabar->hide();

    QAction *actPrevious = new QAction(
                this->style()->standardIcon(QStyle::SP_MediaSkipBackward),
                tr("Previous slide"),
                this);
    QAction *actNext = new QAction(
                this->style()->standardIcon(QStyle::SP_MediaSkipForward),
                tr("Next slide"),
                this);
    QAction *actOptions = new QAction(
                this->style()->standardIcon(QStyle::SP_DialogApplyButton),
                tr("Options"),
                this);
    QAction *actHelp = new QAction(
                this->style()->standardIcon(QStyle::SP_DialogHelpButton),
                tr("Help"),
                this);
    QAction *actGoto = new QAction(
                this->style()->standardIcon(QStyle::SP_FileLinkIcon),
                tr("Goto slide"),
                this);
    QAction *actMode = new QAction(
                this->style()->standardIcon(QStyle::SP_DesktopIcon),
                tr("Presentation/Desktop Mode"),
                this);
    QAction *actQuit = new QAction(
                this->style()->standardIcon(QStyle::SP_BrowserStop),
                tr("Quit"),
                this);

    /* toolbar */
    this->controlbar->addAction(actPrevious);
    this->controlbar->addAction(actNext);
    this->controlbar->addSeparator();
    this->controlbar->addAction(actOptions);
    this->controlbar->addAction(actHelp);
    this->controlbar->addSeparator();
    this->controlbar->addAction(actGoto);
    this->controlbar->addAction(actMode);
    this->controlbar->addAction(actQuit);

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
    glayout->addWidget(this->mediabar,      2, 0, Qt::AlignCenter);
    glayout->addWidget(this->beamerNote,    3, 0, 1, -1, Qt::AlignCenter);
    glayout->addWidget(this->slides,        4, 0, Qt::AlignCenter);
    glayout->addWidget(this->timer,         4, 1, Qt::AlignCenter);
    glayout->addWidget(this->controlbar,    5, 0, 1, -1, Qt::AlignCenter);

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
    QObject::connect(this, SIGNAL(mouseWheelEvent(QWheelEvent*)),
                     this->modele, SLOT(handleMouseWheelModelSequence(QWheelEvent*)));
    QObject::connect(this, SIGNAL(presentationStarted()),
                     this->pTimer, SLOT(startCounterIfNeeded()));
    QObject::connect(this->params, SIGNAL(mainScreenChanged()), SLOT(moveToScreen()));
    QObject::connect(this, SIGNAL(presentationStarted()),
                     this->screensaver, SLOT(dontAllowScreenSaver()));
    QObject::connect(this, SIGNAL(presentationMode()),
                     this->screensaver, SLOT(dontAllowScreenSaver()));
    QObject::connect(this, SIGNAL(desktopMode()),
                     this->screensaver, SLOT(allowScreenSaver()));

    QObject::connect(this->playAction, SIGNAL(triggered()), this->modele, SLOT(startMediaPlayer()));
    QObject::connect(this->pauseAction, SIGNAL(triggered()), this->modele, SLOT(pauseMediaPlayer()));
    QObject::connect(this->stopAction, SIGNAL(triggered()), this->modele, SLOT(stopMediaPlayer()));

    QObject::connect(actPrevious, SIGNAL(triggered()), this->modele, SLOT(gotoPreviousPage()));
    QObject::connect(actNext, SIGNAL(triggered()), this->modele, SLOT(gotoNextPage()));
    QObject::connect(actOptions, SIGNAL(triggered()), SLOT(showOptionsDialog()));
    QObject::connect(actGoto, SIGNAL(triggered()), SLOT(showGotoDialog()));
    QObject::connect(actMode, SIGNAL(triggered()), SLOT(switchDesktopPresentationMode()));
    QObject::connect(actHelp, SIGNAL(triggered()), SLOT(showHelpDialog()));
    QObject::connect(actQuit, SIGNAL(triggered()), SLOT(leaveApplication()));

    this->moveToScreen();
    emit presentationMode();
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

void MainScreenPdfView::leaveApplication()
{
    this->screensaver->allowScreenSaver();
    QCoreApplication::quit();
}

void MainScreenPdfView::switchDesktopPresentationMode()
{
    if (this->maximized) {
        this->showNormal();
        emit desktopMode();
    } else {
        this->showFullScreen();
        emit presentationMode();
    }
}

void MainScreenPdfView::showGotoDialog()
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

void MainScreenPdfView::showOptionsDialog()
{
    if (!this->options) {
        this->options = new OptionsDialog(0, this->params);
        QObject::connect(this->options, SIGNAL(paramsChanged()), SLOT(updateView()));
        QObject::connect(this->options, SIGNAL(paramsChanged()), SLOT(timerUpdated()));
        QObject::connect(this->options, SIGNAL(resetPresentationCounter()), SLOT(resetPresentationTimer()));
    }

    this->options->show();
}

void MainScreenPdfView::showHelpDialog()
{
    /* TODO: implement */
}

void MainScreenPdfView::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat()) {
        ev->ignore();
    } else {
        switch(ev->key())
        {
        case Qt::Key_Escape:
            this->leaveApplication();
            break;

        case Qt::Key_F:
            this->switchDesktopPresentationMode();
            break;

        case Qt::Key_G:
            this->showGotoDialog();
            break;

        case Qt::Key_O:
            this->showOptionsDialog();
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

    if (this->modele->hasMediaFile()) {
        this->mediabar->show();
    } else {
        this->mediabar->hide();
    }
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

void MainScreenPdfView::wheelEvent(QWheelEvent *ev)
{
    emit mouseWheelEvent(ev);
}

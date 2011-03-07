#include "mainscreenpdfview.h"
#include <QtGui/QGridLayout>
#include <QApplication>

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele, Parameters *params) :
    QMainWindow(parent)
{
    QGridLayout *glayout = new QGridLayout(this);
    QWidget *fake = new QWidget(this);
    this->slides = new QLabel(this);
    this->timer = new QLabel(this);
    this->timerLength = new QTimer(this);
    this->currentSlide = new QLabel(this);
    this->nextSlide = new QLabel(this);
    this->timer->setStyleSheet("color: white; font-weight: bold;");
    this->setStyleSheet("background-color: black;");

    glayout->addWidget(this->currentSlide, 0, 0, Qt::AlignCenter);
    glayout->addWidget(this->nextSlide, 0, 3, Qt::AlignCenter);
    glayout->addWidget(this->slides, 2, 1, Qt::AlignCenter);
    glayout->addWidget(this->timer, 2, 2, Qt::AlignCenter);

    fake->setLayout(glayout);
    this->setCentralWidget(fake);

    this->modele = modele;
    this->params = params;
    this->presentationEmergency = this->params->getPresentationEmergency();
    this->presentationLength = this->params->getPresentationLength();
    this->timerInterval = 1000;
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

        case Qt::Key_Space:
            this->startTimer(this->timerInterval);
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

    this->updateTimerView();

    float cw = (QApplication::desktop()->screenGeometry(this).width() * 0.7) / this->modele->getPageSize().width();
    float ch = (QApplication::desktop()->screenGeometry(this).height() * 0.7) / this->modele->getPageSize().height();
    float f1 = qMin(cw, ch);

    float nw = (QApplication::desktop()->screenGeometry(this).width() * 0.4) / this->modele->getPageSize().width();
    float nh = (QApplication::desktop()->screenGeometry(this).height() * 0.4) / this->modele->getPageSize().height();
    float f2 = qMin(nw, nh);

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

void MainScreenPdfView::timerEvent(QTimerEvent *timer)
{
    this->presentationLength -= this->timerInterval;
    this->updateTimerView();
    if (this->presentationLength <= 0) {
        this->killTimer(timer->timerId());
    }
}

void MainScreenPdfView::updateTimerView()
{
    int time_heures = this->presentationLength / (1000*60*60);
    int time_minutes = (this->presentationLength % (1000*60*60)) / (1000*60);
    int time_secondes = ((this->presentationLength % (1000*60*60)) % (1000*60)) / 1000;

    QTime temps(time_heures, time_minutes, time_secondes, 0);
    QString s_temps(temps.toString("hh:mm:ss"));

    this->timer->setText(s_temps);
    if (this->presentationLength <= this->presentationEmergency) {
        this->timer->setStyleSheet("color: red; font-weight: bold");
    }
}

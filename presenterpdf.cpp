#include "presenterpdf.h"
#include <stdio.h>

PresenterPdf::PresenterPdf(QWidget *parent, PDFModel *modele, Parameters *params) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect res = desktop->availableGeometry(params->getProjectorScreenId());
    this->move(res.x(), res.y());
    this->showFullScreen();
    this->setStyleSheet("background-color: black;");

    // QGridLayout *glayout = new QGridLayout(this);
    // QWidget *fake = new QWidget(this);
    this->imgLabel = new QLabel();

    // glayout->addWidget(this->imgLabel, 0, 0, Qt::AlignCenter);
    // fake->setLayout(glayout);
    this->setCentralWidget(this->imgLabel);

    this->modele = modele;
    this->params = params;
    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
    QObject::connect(this, SIGNAL(keyPressed(QKeyEvent*)),
                     this->modele, SLOT(handleModelSequence(QKeyEvent*)));
}

void PresenterPdf::updateView() {
    this->displayPage = this->modele->getImgCurrentPage();
    this->imgLabel->setPixmap(QPixmap::fromImage(this->displayPage));
}

void PresenterPdf::keyReleaseEvent(QKeyEvent *ev)
{
    if (ev->isAutoRepeat()) {
        ev->ignore();
    } else {
        switch(ev->key())
        {
        case Qt::Key_Escape:
            QCoreApplication::quit();
            break;

        default:
            emit keyPressed(ev);
            break;
        }
    }
}

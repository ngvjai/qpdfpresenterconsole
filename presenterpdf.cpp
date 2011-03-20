#include "presenterpdf.h"

PresenterPdf::PresenterPdf(QWidget *parent, PDFModel *modele, Parameters *params) :
    QMainWindow(parent)
{
    this->modele = modele;
    this->params = params;

    this->imgLabel = new QLabel();
    this->setCentralWidget(this->imgLabel);
    this->centralWidget()->setStyleSheet("background-color: black;");

    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
    QObject::connect(this, SIGNAL(keyPressed(QKeyEvent*)),
                     this->modele, SLOT(handleModelSequence(QKeyEvent*)));
    QObject::connect(this->params, SIGNAL(projectorScreenChanged()), SLOT(moveToScreen()));

    this->moveToScreen();
}

void PresenterPdf::moveToScreen()
{
    this->showNormal();
    QRect res = QApplication::desktop()->screenGeometry(this->params->getProjectorScreenId());
    this->move(res.x(), res.y());
    this->resize(QApplication::desktop()->availableGeometry(this).size());
    this->showFullScreen();
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

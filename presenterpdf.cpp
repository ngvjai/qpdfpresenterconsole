#include "presenterpdf.h"
#include <stdio.h>

PresenterPdf::PresenterPdf(QWidget *parent, PDFModel *modele) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    QRect res = desktop->availableGeometry(2);
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
    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
}

void PresenterPdf::updateView() {
    this->displayPage = this->modele->getImgCurrentPage();
    this->imgLabel->setPixmap(QPixmap::fromImage(this->displayPage));
}

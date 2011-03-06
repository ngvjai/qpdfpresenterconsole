#include "presenterpdf.h"

PresenterPdf::PresenterPdf(QWidget *parent, PDFModel *modele) :
    QMainWindow(parent)
{
    QApplication *app = (QApplication*)parent;
    QDesktopWidget *desktop = app->desktop();
    QRect res = desktop->screenGeometry(2);
    this->move(QPoint(res.x(), res.y()));
    this->resize(res.width(), res.height());
    this->modele = modele;

    QObject::connect(this->modele, SIGNAL(renderingChanged()), SLOT(updateView()));
}

void PresenterPdf::updateView() {
    this->displayPage = this->modele->getImgCurrentPage();
    QLabel *imgLabel = new QLabel();
    imgLabel->setPixmap(QPixmap::fromImage(this->displayPage));
    this->setCentralWidget(imgLabel);
}

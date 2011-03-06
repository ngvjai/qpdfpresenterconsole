#include "mainscreenpdfview.h"

MainScreenPdfView::MainScreenPdfView(QWidget *parent, PDFModel *modele) :
    QMainWindow(parent)
{
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
            modele->gotoPreviousPage();
            break;

        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Enter:
            modele->gotoNextPage();
            break;

        default:
            break;
        }
    }
}

void MainScreenPdfView::updateView()
{

}

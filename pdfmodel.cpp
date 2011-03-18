#include "pdfmodel.h"
#include <QApplication>
#include <QFileDialog>

PDFModel::PDFModel(QObject *parent, Parameters *params, PresentationTimer *timer) :
    QObject(parent)
{
    QDesktopWidget *desktop = (QDesktopWidget *)parent;
    this->params = params;
    this->timer = timer;

    if (this->params->getPdfFileName() == "") {
        QString fileName = QFileDialog::getOpenFileName(0,
             tr("Open PDF file"), "", tr("PDF Files (*.pdf)"));
        if (fileName != "") {
            this->params->setPdfFileName(fileName);
        } else {
            QApplication::exit();
        }
    }

    this->setPdfFileName(this->params->getPdfFileName());

    this->firstPage = 0;
    this->currentPage = 0;
    this->dpiX = 72.0;
    this->dpiY = 72.0;
    this->projectorSize = desktop->screenGeometry(this->params->getProjectorScreenId());

    this->document = Poppler::Document::load(this->getPdfFileName());
    if (!this->document || this->document->isLocked()) {
      delete this->document;
      return;
    }

    // Pages starts at 0 ...
    this->lastPage = this->document->numPages() - 1;
    this->pageSize = this->document->page(0)->pageSizeF();
    this->scaleFactorX = this->projectorSize.width() / this->pageSize.width();
    this->scaleFactorY = this->projectorSize.height() / this->pageSize.height();

    QObject::connect(this, SIGNAL(presentationStarted()), this->timer, SLOT(startCounterIfNeeded()));
    QObject::connect(this, SIGNAL(presentationReset()), this->timer, SLOT(resetCounter()));
}

PDFModel::~PDFModel()
{
    delete this->document;
}

bool PDFModel::pdfLoaded()
{
    return (this->document != NULL);
}

int PDFModel::getCurrentPage()
{
    return this->currentPage;
}

int PDFModel::getLastPage()
{
    return this->lastPage;
}

int PDFModel::getPreviousPage()
{
    return (this->getCurrentPage() > this->firstPage ? ((this->getCurrentPage() - 1) % (this->lastPage + 1)) : this->firstPage);
}

int PDFModel::getNextPage()
{
    return (this->getCurrentPage() < this->lastPage ? ((this->getCurrentPage() + 1) % (this->lastPage + 1)) : this->lastPage);
}

QImage PDFModel::renderPdfPage(int page, QSizeF scaleFactor)
{
    QImage image;

    // Access page of the PDF file
    if (page >= this->firstPage && page <= this->lastPage) {
        this->document->setRenderHint(Poppler::Document::Antialiasing, true);
        this->document->setRenderHint(Poppler::Document::TextAntialiasing, true);
        this->document->setRenderHint(Poppler::Document::TextHinting, true);
        Poppler::Page* pdfPage = this->document->page(page);  // Document starts at page 0

        if (pdfPage == NULL) {
        }

        QList<Poppler::Annotation*> annotations = pdfPage->annotations();
        QList<Poppler::Annotation*>::iterator annIt;
        bool found = false;
        for(annIt = annotations.begin(); annIt != annotations.end(); ++annIt) {
            if ( (*annIt)->subType() == Poppler::TextAnnotation::AText
                && ((Poppler::TextAnnotation*)(*annIt))->textIcon() == BEAMER_NOTE_NAME ) {
                Poppler::TextAnnotation *te = (Poppler::TextAnnotation*) (*annIt);
                this->setCurrentBeamerNote(te->contents());
                found = true;
            }
        }

        if (!found) {
            this->setCurrentBeamerNote("");
        }

        // Generate a QImage of the rendered page
        image = pdfPage->renderToImage(
                scaleFactor.width() * this->dpiX,
                scaleFactor.height() * this->dpiY
                );
        if (image.isNull()) {
        }

        // after the usage, the page must be deleted
        delete pdfPage;
    }

    return image;
}

QImage PDFModel::renderPdfPage(int page)
{
    return this->renderPdfPage(page, QSizeF(this->scaleFactorX, this->scaleFactorY));
}

void PDFModel::renderPreviousPage()
{
    this->imgPreviousPage = this->renderPdfPage(this->getPreviousPage());
}

void PDFModel::renderCurrentPage()
{
    this->imgCurrentPage = this->renderPdfPage(this->getCurrentPage());
}

void PDFModel::renderNextPage()
{
    this->imgNextPage = this->renderPdfPage(this->getNextPage());
}

void PDFModel::render()
{
    this->renderCurrentPage();
    this->renderPreviousPage();
    this->renderNextPage();

    emit renderingChanged();
}

void PDFModel::gotoNextPage()
{
    this->currentPage = this->getNextPage();
    this->imgCurrentPage = this->imgNextPage;
    emit renderingChanged();
    this->renderNextPage();
}

void PDFModel::gotoPreviousPage()
{
    this->currentPage = this->getPreviousPage();
    this->imgCurrentPage = this->imgPreviousPage;
    emit renderingChanged();
    this->renderPreviousPage();
}

void PDFModel::gotoFirstPage()
{
    this->currentPage = this->firstPage;
    this->render();
}

void PDFModel::gotoLastPage()
{
    this->currentPage = this->lastPage;
    this->render();
}

void PDFModel::gotoOpenPage()
{
    int openPage = this->params->getOpenPage();
    if (openPage >= this->firstPage && openPage <= this->lastPage) {
        this->currentPage = openPage;
        this->render();
    }
}

QImage PDFModel::getImgPreviousPage()
{
    return this->imgPreviousPage;
}

QImage PDFModel::getImgCurrentPage()
{
    return this->imgCurrentPage;
}

QImage PDFModel::getImgNextPage()
{
    return this->imgNextPage;
}

QString PDFModel::getCurrentBeamerNote()
{
    return this->currentBeamerNote;
}

void PDFModel::setCurrentBeamerNote(QString v)
{
    this->currentBeamerNote = v;
}

QSizeF PDFModel::getScaleFactor()
{
    return QSizeF((qreal)this->scaleFactorX, (qreal)this->scaleFactorY);
}

QSizeF PDFModel::getPageSize()
{
    return this->pageSize;
}

void PDFModel::setPdfFileName(QString file)
{
    this->pdfFileName = file;
}

QString PDFModel::getPdfFileName(void)
{
    return this->pdfFileName;
}

void PDFModel::handleModelSequence(QKeyEvent *ev)
{
    if (ev->isAutoRepeat()) {
        ev->ignore();
    } else {
        switch(ev->key())
        {
        case Qt::Key_Left:
        case Qt::Key_Down:
        case Qt::Key_PageUp:
            emit presentationStarted();
            this->gotoPreviousPage();
            break;

        case Qt::Key_Right:
        case Qt::Key_Up:
        case Qt::Key_Enter:
        case Qt::Key_PageDown:
            emit presentationStarted();
            this->gotoNextPage();
            break;

        case Qt::Key_Home:
            emit presentationReset();
            this->gotoFirstPage();
            break;

        case Qt::Key_End:
            this->gotoLastPage();
            break;

        default:
            break;
        }
    }
}

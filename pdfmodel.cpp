#include "pdfmodel.h"
#include <stdio.h>

PDFModel::PDFModel(QObject *parent, Parameters *params) :
    QObject(parent)
{
    QDesktopWidget *desktop = (QDesktopWidget *)parent;
    this->params = params;
    this->setPdfFileName(this->params->getPdfFileName());

    this->firstPage = 0;
    this->currentPage = 0;
    this->dpiX = desktop->physicalDpiX();
    this->dpiY = desktop->physicalDpiY();
    this->projectorSize = desktop->screenGeometry(2);

    this->document = Poppler::Document::load(this->getPdfFileName());
    if (!this->document || this->document->isLocked()) {
      delete this->document;
      return;
    }

    // Pages starts at 0 ...
    this->lastPage = this->document->numPages() - 1;
    this->pageSize = this->document->page(0)->pageSizeF();
    this->scaleFactorX = this->projectorSize.height() / this->pageSize.height();
    this->scaleFactorY = this->projectorSize.width() / this->pageSize.width();
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
    return (this->getCurrentPage() > this->firstPage ? ((this->getCurrentPage() - 1) % this->lastPage) : this->firstPage);
}

int PDFModel::getNextPage()
{
    return (this->getCurrentPage() < this->lastPage ? ((this->getCurrentPage() + 1) % this->lastPage) : this->lastPage);
}

QImage PDFModel::renderPdfPage(int page)
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

        // Generate a QImage of the rendered page
        image = pdfPage->renderToImage( this->scaleFactorX * 72.0, this->scaleFactorY * 72.0 );
        if (image.isNull()) {
        }

        // after the usage, the page must be deleted
        delete pdfPage;
    }

    return image;
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
    this->render();
}

void PDFModel::gotoPreviousPage()
{
    this->currentPage = this->getPreviousPage();
    this->render();
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

QImage PDFModel::getImgPreviousPage()
{
    return this->imgPreviousPage.copy(0, 0,
                                     this->imgPreviousPage.width(),
                                     this->imgPreviousPage.height()
                                     );
}

QImage PDFModel::getImgCurrentPage()
{
    return this->imgCurrentPage.copy(0, 0,
                                     this->imgCurrentPage.width(),
                                     this->imgCurrentPage.height()
                                     );
}

QImage PDFModel::getImgNextPage()
{
    return this->imgNextPage.copy(0, 0,
                                     this->imgNextPage.width(),
                                     this->imgNextPage.height()
                                     );
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

#include "pdfmodel.h"
#include <QApplication>
#include <QFileDialog>
#include <QtConcurrentRun>

PDFModel::PDFModel(QObject *parent, Parameters *params, PresentationTimer *timer) :
    QObject(parent)
{
    this->params = params;
    this->timer = timer;
    this->document = NULL;

    if (!this->params->getPdfFileName().isEmpty()) {
        this->setPdfFileName(this->params->getPdfFileName());
        this->finishInit();
    } else {
        QString fileName = QFileDialog::getOpenFileName(0,
             tr("Open PDF file"), "", tr("PDF Files (*.pdf)"));
        if (!fileName.isEmpty()) {
            this->params->setPdfFileName(fileName);
            this->setPdfFileName(fileName);
            this->finishInit();
        } else {
            QCoreApplication::exit(EXIT_FAILURE);
        }
    }
}

void PDFModel::finishInit()
{
    this->document = Poppler::Document::load(this->getPdfFileName());

    if (!this->document || this->document->isLocked()) {
      delete this->document;
      return;
    }

    this->firstPage = 0;
    this->currentPage = 0;
    // Pages starts at 0 ...
    this->lastPage = this->document->numPages() - 1;
    this->pageSize = this->document->page(0)->pageSizeF();
    this->updateProjectorSize();

    QObject::connect(this, SIGNAL(presentationStarted()), this->timer, SLOT(startCounterIfNeeded()));
    QObject::connect(this, SIGNAL(presentationReset()), this->timer, SLOT(resetCounter()));
    QObject::connect(this->params, SIGNAL(projectorScreenChanged()), SLOT(updateProjectorSize()));
}

void PDFModel::updateProjectorSize()
{
    this->projectorSize = QApplication::desktop()->screenGeometry(this->params->getProjectorScreenId());
    this->dpiX = 72.0;
    this->dpiY = 72.0;
    this->scaleFactorX = this->projectorSize.width() / this->pageSize.width();
    this->scaleFactorY = this->projectorSize.height() / this->pageSize.height();
    this->scaleFactor = qMin(this->scaleFactorX, this->scaleFactorY);
    this->render();
}

PDFModel::~PDFModel()
{
    delete this->document;
}

bool PDFModel::pdfLoaded()
{
    return (this->document != NULL);
}

int PDFModel::getFirstPage()
{
    return this->firstPage;
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
    int v = (this->getCurrentPage() > this->firstPage ? ((this->getCurrentPage() - 1) % (this->lastPage + 1)) : this->firstPage);
    return v;
}

int PDFModel::getNextPage()
{
    int v = (this->getCurrentPage() < this->lastPage ? ((this->getCurrentPage() + 1) % (this->lastPage + 1)) : this->lastPage);
    return v;
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
        for(annIt = annotations.begin(); annIt != annotations.end(); ++annIt) {
            if ( (*annIt)->subType() == Poppler::TextAnnotation::AText
                && ((Poppler::TextAnnotation*)(*annIt))->textIcon() == BEAMER_NOTE_NAME ) {
                Poppler::TextAnnotation *te = (Poppler::TextAnnotation*) (*annIt);
                this->setBeamerNote(te->contents(), page);
            }
        }

        // Generate a QImage of the rendered page
        image = pdfPage->renderToImage(scaleFactor.width() * this->dpiX, scaleFactor.height() * this->dpiY);
        if (image.isNull()) {
        }

        // after the usage, the page must be deleted
        delete pdfPage;
    }

    return image;
}

QImage PDFModel::renderPdfPage(int page)
{
    return this->renderPdfPage(page, QSizeF(this->scaleFactor, this->scaleFactor));
}

void PDFModel::renderPreviousPage()
{
    this->mutexPreviousPage.lockForWrite();
        this->imgPreviousPage = this->renderPdfPage(this->getPreviousPage());
    this->mutexPreviousPage.unlock();
}

void PDFModel::renderCurrentPage()
{
    this->mutexCurrentPage.lockForWrite();
        this->imgCurrentPage = this->renderPdfPage(this->getCurrentPage());
    this->mutexCurrentPage.unlock();
}

void PDFModel::renderNextPage()
{
    this->mutexNextPage.lockForWrite();
        this->imgNextPage = this->renderPdfPage(this->getNextPage());
    this->mutexNextPage.unlock();
}

void PDFModel::render()
{
    this->renderCurrentPage();
    this->renderPreviousPage();
    this->renderNextPage();
    emit renderingChanged();
}

void PDFModel::gotoSpecificPage(int page)
{
    if (page >= this->firstPage && page <= this->lastPage) {
        this->currentPage = page;
        this->render();
    }
}

void PDFModel::gotoNextPage()
{
    if (this->getCurrentPage() != this->getLastPage()) {
        this->currentPage = this->getNextPage();

        this->mutexPreviousPage.lockForRead(); this->mutexCurrentPage.lockForRead();
        this->imgPreviousPage = this->imgCurrentPage;
        this->mutexPreviousPage.unlock(); this->mutexNextPage.lockForRead();
        this->imgCurrentPage = this->imgNextPage;
        this->mutexCurrentPage.unlock(); this->mutexNextPage.unlock();

        emit renderingChanged();
        if (this->getCurrentPage() < this->getLastPage()) {
            QFuture<void> future = QtConcurrent::run(this, &PDFModel::renderNextPage);
        }
    }
}

void PDFModel::gotoPreviousPage()
{
    if (this->getCurrentPage() != this->getFirstPage()) {
        this->currentPage = this->getPreviousPage();

        this->mutexNextPage.lockForRead(); this->mutexCurrentPage.lockForRead();
        this->imgNextPage = this->imgCurrentPage;
        this->mutexNextPage.unlock(); this->mutexPreviousPage.lockForRead();
        this->imgCurrentPage = this->imgPreviousPage;
        this->mutexCurrentPage.unlock(); this->mutexPreviousPage.unlock();

        emit renderingChanged();
        QFuture<void> future = QtConcurrent::run(this, &PDFModel::renderPreviousPage);
    }
}

void PDFModel::gotoFirstPage()
{
    this->gotoSpecificPage(this->firstPage);
}

void PDFModel::gotoLastPage()
{
    this->gotoSpecificPage(this->lastPage);
}

void PDFModel::gotoOpenPage()
{
    this->gotoSpecificPage(this->params->getOpenPage());
}

QImage& PDFModel::getImgPreviousPage()
{
    return this->imgPreviousPage;
}

QImage& PDFModel::getImgCurrentPage()
{
    return this->imgCurrentPage;
}

QImage& PDFModel::getImgNextPage()
{
    return this->imgNextPage;
}

QString PDFModel::getCurrentBeamerNote()
{
    QString s = "";
    if (this->annotations.contains(this->getCurrentPage())) {
        s = this->annotations.value(this->getCurrentPage());
    }
    return s;
}

void PDFModel::setBeamerNote(QString v, int page)
{
    if (!this->annotations.contains(page)) {
        this->annotations.insert(page, v);
    }
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

void PDFModel::handleKeyModelSequence(QKeyEvent *ev)
{
    if (ev) {
        if (ev->isAutoRepeat()) {
            ev->ignore();
        } else {
            switch(ev->key())
            {
            case Qt::Key_Left:
            case Qt::Key_Up:
            case Qt::Key_PageUp:
                emit presentationStarted();
                this->gotoPreviousPage();
                break;

            case Qt::Key_Enter:
            case Qt::Key_Right:
            case Qt::Key_Down:
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
}

void PDFModel::handleMouseModelSequence(QMouseEvent *ev)
{
    if (ev) {
        switch(ev->button()) {
        case Qt::RightButton:
            emit presentationStarted();
            this->gotoPreviousPage();
            break;

        case Qt::LeftButton:
            emit presentationStarted();
            this->gotoNextPage();
            break;

        default:
            break;
        }
    }
}

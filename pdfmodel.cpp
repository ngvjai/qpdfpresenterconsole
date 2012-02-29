#include "pdfmodel.h"
#include <QApplication>
#include <QFileDialog>
#include <QtConcurrentRun>

#include <iostream>

PDFModel::PDFModel(QObject *parent, Parameters *params, PresentationTimer *timer) :
    QObject(parent)
{
    this->ContentPart = 0;
    this->AnnotationsPart = 1;
    this->params = params;
    this->timer = timer;
    this->document = NULL;
    this->textannot = NULL;

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
    this->updateTextAnnot();

    QObject::connect(this, SIGNAL(presentationStarted()), this->timer, SLOT(startCounterIfNeeded()));
    QObject::connect(this, SIGNAL(presentationReset()), this->timer, SLOT(resetCounter()));
    QObject::connect(this->params, SIGNAL(projectorScreenChanged()), SLOT(updateProjectorSize()));
    QObject::connect(this->params, SIGNAL(beamerNotesChanged()), SLOT(updateProjectorSize()));
    QObject::connect(this->params, SIGNAL(textAnnotChanged()), SLOT(updateTextAnnot()));
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

void PDFModel::updateTextAnnot()
{
    if (this->textannot != NULL) {
        delete this->textannot;
    } else {
        if (this->params->getTextAnnot()) {
            this->mutexTextAnnot.lockForWrite();
            this->textannot = new TextAnnot(this);

            if (this->textannot->exists(this->getPdfFileName())) {
                this->textannot->processAnnotations();
            } else {
                QString files = this->textannot->expectedFilenames(this->getPdfFileName()).join(";");
                QString err = QObject::tr("Text file annotations for pdf '%1' not found. Cannot use text file annotations, disabling for now. File(s) expected: %2")
                            .arg(this->getPdfFileName(), files);
                QMessageBox::warning(0, APPNAME, err);
                this->params->setTextAnnot(false, false);
            }

            this->mutexTextAnnot.unlock();
            this->updateProjectorSize();
        }
    }
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

QImage PDFModel::renderPdfPage(int page, QSizeF scaleFactor, int partie)
{
    QImage image;
    int annotscale = 1;

    // Access page of the PDF file
    if (page >= this->firstPage && page <= this->lastPage) {
        this->document->setRenderHint(Poppler::Document::Antialiasing, true);
        this->document->setRenderHint(Poppler::Document::TextAntialiasing, true);
        this->document->setRenderHint(Poppler::Document::TextHinting, true);
        Poppler::Page* pdfPage = this->document->page(page);  // Document starts at page 0

        if (pdfPage == NULL) {
        }

        if (this->params->getBeamerNotes()) {
            annotscale = 2;
        }

        // Generate a QImage of the rendered page
        image = pdfPage->renderToImage(
                    scaleFactor.width() * this->dpiX * annotscale,
                    scaleFactor.height() * this->dpiY * annotscale);

        if (!image.isNull()) {
            if (partie == this->getContentPart()) {
                if (this->params->getBeamerNotes() && this->params->getBeamerNotesPart() == BEAMER_NOTES_RIGHT) {
                    image = image.copy(0, 0, image.width() / annotscale, image.height());
                }
                if (this->params->getBeamerNotes() && this->params->getBeamerNotesPart() == BEAMER_NOTES_LEFT) {
                    image = image.copy(image.width() / annotscale, 0, image.width() / annotscale, image.height());
                }
            }

            if (partie == this->getAnnotationsPart()) {
                if (this->params->getBeamerNotes() && this->params->getBeamerNotesPart() == BEAMER_NOTES_RIGHT) {
                    image = image.copy(image.width() / annotscale, 0, image.width() / annotscale, image.height());
                }
                if (this->params->getBeamerNotes() && this->params->getBeamerNotesPart() == BEAMER_NOTES_LEFT) {
                    image = image.copy(0, 0, image.width() / annotscale, image.height());
                }
            }
        }

        if (page == this->getCurrentPage()) {
            this->processCurrentPageAnnotations(pdfPage);
        }

        // after the usage, the page must be deleted
        delete pdfPage;
    }

    return image;
}

void PDFModel::processCurrentPageAnnotations(Poppler::Page *pdfPage)
{
    if (pdfPage) {
        if (!pdfPage->annotations().isEmpty()) {
            QList<Poppler::Annotation*> annotations = pdfPage->annotations();
            QList<Poppler::Annotation*>::iterator it;

            for (it = annotations.begin(); it != annotations.end(); ++it) {
                Poppler::Annotation* annot = (*it);

                switch(annot->subType()) {
                    /**
                      * Does not appear to be set by Beamer:
                      * \include{movie15}
                      * [...]
                      * \begin{figure}[ht]
                      *     \includemovie[
                      *     poster,
                      *     text={\small(haha.mp4)}
                      *     ]{6cm}{6cm}{haha.mp4}
                      *     \end{figure}
                      **/
                    case Poppler::Annotation::AMovie:
                        {
                            Poppler::MovieAnnotation* movannot = (Poppler::MovieAnnotation*) annot;
                            Poppler::MovieObject* movobj = movannot->movie();
                            std::cerr << "AMovie::" << movannot->movieTitle().toStdString() << std::endl;
                            std::cerr << "MovieObject::" << movobj->url().toStdString() << std::endl;
                        }
                        break;

                    case Poppler::Annotation::AFileAttachment:
                        {
                            Poppler::FileAttachmentAnnotation* fileannot = (Poppler::FileAttachmentAnnotation*) annot;
                            Poppler::EmbeddedFile* file = fileannot->embeddedFile();
                            std::cerr << "AFileAttachment:" << file->data().size() << std::endl;
                        }
                        break;

                    default:
                        break;
                }
            }

            QList<Poppler::Link*> links = pdfPage->links();
            QList<Poppler::Link*>::iterator lit;
            for (lit = links.begin(); lit != links.end(); ++lit) {
                Poppler::Link* link = (*lit);
                if (link->linkType() == Poppler::Link::Goto) {
                    std::cerr << "ALink::" << link->linkArea().x() << std::endl;
                    this->gotoLinks.append(link);
                }
            }
        }
    }
}

QImage PDFModel::renderPdfPage(int page)
{
    return this->renderPdfPage(page, QSizeF(this->scaleFactor, this->scaleFactor), PDFModel::ContentPart);
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

QString PDFModel::getCurrentTextAnnot()
{
    QString result;
    if (this->params->getTextAnnot()) {
        this->mutexTextAnnot.lockForRead();
        result = this->textannot->getTextAnnot(this->getCurrentPage());
        this->mutexTextAnnot.unlock();
    }
    return result;
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

QString& PDFModel::getPdfFileName(void)
{
    return this->pdfFileName;
}

int PDFModel::getContentPart()
{
    return this->ContentPart;
}

int PDFModel::getAnnotationsPart()
{
    return this->AnnotationsPart;
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

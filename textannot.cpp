#include "textannot.h"
#include <QFileInfo>
#include <QStringList>

TextAnnot::TextAnnot(QObject *parent) :
    QObject(parent)
{
    this->extensionTarget = QRegExp("\\.pdf$");
    this->extensions.append(".txt");
    this->extensions.append(".annot");
    this->annotationsFile = "";
}

TextAnnot::~TextAnnot()
{
}

bool TextAnnot::exists(QString &file)
{
    QStringList targets = this->expectedFilenames(file);
    QStringList::iterator target;
    for(target = targets.begin(); target != targets.end(); ++target) {
        QFileInfo fi(*target);
        if (fi.exists()) {
            this->annotationsFile = *target;
            return true;
        }
    }

    return false;
}

QStringList TextAnnot::expectedFilenames(QString file) {
    QStringList files;
    QStringList::iterator extIter;
    for(extIter = this->extensions.begin(); extIter != this->extensions.end(); ++extIter) {
        QString orig = file;
        files.append(orig.replace(this->extensionTarget, *extIter));
    }

    return files;
}

void TextAnnot::processAnnotations(void)
{
    QFile source(this->annotationsFile);
    int fd;

    if (source.exists() && source.open(fd, QIODevice::ReadOnly)) {

    }

    source.close();
}

QString TextAnnot::getTextAnnot(int page)
{
    QString s = "";
    this->mutexTextAnnot.lockForRead();
    if (!this->annotations.isEmpty()) {
        if (this->annotations.contains(page)) {
            s = this->annotations.value(page);
        }
    }
    this->mutexTextAnnot.unlock();
    return s;
}

void TextAnnot::setTextAnnot(int page, QString v)
{
    this->mutexTextAnnot.lockForWrite();
    if (!this->annotations.contains(page)) {
        this->annotations.insert(page, v);
    }
    this->mutexTextAnnot.unlock();
}

void TextAnnot::prepareToDelete(void)
{
    this->mutexTextAnnot.lockForWrite();
}

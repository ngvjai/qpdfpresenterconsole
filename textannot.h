#ifndef TEXTANNOT_H
#define TEXTANNOT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QRegExp>
#include <QReadWriteLock>
#include <QHash>

class TextAnnot : public QObject
{
    Q_OBJECT
    QRegExp extensionTarget;
    QStringList extensions;

    QString annotationsFile;
    QHash<int, QString> annotations;

    QReadWriteLock mutexTextAnnot;

public:
    explicit TextAnnot(QObject *parent = 0);
    ~TextAnnot();
    bool exists(QString &file);
    void processAnnotations(void);
    QString getTextAnnot(int page);
    QStringList expectedFilenames(QString file);
    void setTextAnnot(int page, QString v);
    void prepareToDelete(void);

signals:

public slots:

};

#endif // TEXTANNOT_H

#ifndef MAINSCREENPDFVIEW_H
#define MAINSCREENPDFVIEW_H

#include <QMainWindow>
#include <QtGui/QKeyEvent>
#include "pdfmodel.h"

class MainScreenPdfView : public QMainWindow
{
    Q_OBJECT
    PDFModel *modele;

public:
    explicit MainScreenPdfView(QWidget *parent = 0, PDFModel *modele = 0);

signals:

public slots:
    void keyReleaseEvent(QKeyEvent *ev);
    void updateView(void);

};

#endif // MAINSCREENPDFVIEW_H

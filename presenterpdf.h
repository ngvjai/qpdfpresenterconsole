/* vim: set et ts=4 sw=4: */

#ifndef PRESENTERPDF_H
#define PRESENTERPDF_H

#include <QMainWindow>
#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLayout>
#include "slidewidget.h"
#include "pdfmodel.h"
#include "parameters.h"

class PresenterPdf : public QMainWindow
{
    Q_OBJECT
    QImage displayPage;
    QLabel *imgLabel;
    PDFModel *modele;
    Parameters *params;

public:
    explicit PresenterPdf(QWidget *parent = 0, PDFModel *modele = 0, Parameters *params = 0);

signals:
    void keyPressed(QKeyEvent *e);

public slots:
    void keyReleaseEvent(QKeyEvent *ev);
    void updateView(void);
    void moveToScreen(void);

};

#endif // PRESENTERPDF_H

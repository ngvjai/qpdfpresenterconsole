#ifndef PRESENTERPDF_H
#define PRESENTERPDF_H

#include <QMainWindow>
#include <QtGui>
#include <QtGui/QApplication>
#include <QtGui/QDesktopWidget>
#include <QtGui/QLayout>
#include "pdfmodel.h"

class PresenterPdf : public QMainWindow
{
    Q_OBJECT
    QImage displayPage;
    QLabel *imgLabel;
    PDFModel *modele;

public:
    explicit PresenterPdf(QWidget *parent = 0, PDFModel *modele = 0);

signals:

public slots:
    void updateView(void);

};

#endif // PRESENTERPDF_H

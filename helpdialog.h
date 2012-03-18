/* vim: set et ts=4 sw=4: */

#ifndef HELPDIALOG_H
#define HELPDIALOG_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QtWebKit/QWebView>
#include "app.h"

class HelpDialog : public QMainWindow
{
    Q_OBJECT

public:
    explicit HelpDialog(QWidget *parent = 0);

signals:

public slots:

};

#endif // HELPDIALOG_H

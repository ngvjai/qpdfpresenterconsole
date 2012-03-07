#include "helpdialog.h"
#include <QDir>

HelpDialog::HelpDialog(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *central = new QWidget(this);
    QGridLayout *glayout = new QGridLayout(this);

    QLabel *nameVersion = new QLabel(orgname + " v" + appvers, this);
    QWebView *html = new QWebView(this);

    QFont f = nameVersion->font();
    f.setPointSize(16);
    nameVersion->setFont(f);

    glayout->addWidget(nameVersion, 0, 0, Qt::AlignCenter);
    glayout->addWidget(html,        1, 0, Qt::AlignCenter);

    central->setLayout(glayout);
    this->setCentralWidget(central);

    html->load(
                QUrl(
                    QDir::toNativeSeparators(
                        "file://"
                        + QString(DATADIR)
                        + QDir::separator()
                        + QString(SHORTNAME) + ".html")
                    )
                );
}

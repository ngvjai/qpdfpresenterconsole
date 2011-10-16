#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "parameters.h"
#include "app.h"

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    Parameters *params;

    int oldPresentationLength;
    int oldPresentationEmergency;
    int oldMainScreenId;
    int oldProjectorScreenId;
    bool oldBeamerNotes;
    QString oldBeamerNotesPart;
    float oldCurrentSlidePrcentWidth;

    int getHours(int v);
    int getMinutes(int v);
    int getSeconds(int v);

public:
    explicit OptionsDialog(QWidget *parent = 0, Parameters *params = 0);
    ~OptionsDialog();

signals:
    void paramsChanged();
    void resetPresentationCounter();

private:
    Ui::OptionsDialog *ui;

private slots:
    void on_buttonBox_rejected();
    void on_buttonBox_accepted();
    void on_sliderSlidesWidth_valueChanged(int value);
    void on_chkBeamerNotes_stateChanged(int arg1);
};

#endif // OPTIONSDIALOG_H

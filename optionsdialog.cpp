#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent, Parameters *params) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    this->ui->setupUi(this);

    this->params = params;
    this->oldCurrentSlidePrcentWidth = this->params->getCurrentSlidePrcentWidth();
    this->oldMainScreenId = this->params->getMainScreenId();
    this->oldPresentationEmergency = this->params->getPresentationEmergency();
    this->oldPresentationLength = this->params->getPresentationLength();
    this->oldProjectorScreenId = this->params->getProjectorScreenId();
    this->oldBeamerNotes = this->params->getBeamerNotes();
    this->oldBeamerNotesPart = this->params->getBeamerNotesPart();
    this->oldTextAnnot = this->params->getTextAnnot();

    this->setWindowTitle(QObject::tr("Parameters management"));

    this->ui->timePresentationLength->setTime(
            QTime(
                    this->getHours(this->params->getPresentationLength()),
                    this->getMinutes(this->params->getPresentationLength()),
                    this->getSeconds(this->params->getPresentationLength())
                  )
            );

    this->ui->timePresentationEmergency->setTime(
            QTime(
                    this->getHours(this->params->getPresentationEmergency()),
                    this->getMinutes(this->params->getPresentationEmergency()),
                    this->getSeconds(this->params->getPresentationEmergency())
                  )
            );

    this->ui->sliderSlidesWidth->setValue(this->params->getCurrentSlidePrcentWidth() * 100);

    this->ui->mainScreenIdentifier->setText(QString("%1").arg(this->params->getMainScreenId()));
    this->ui->projectorScreenIdentifier->setText(QString("%1").arg(this->params->getProjectorScreenId()));
    this->ui->chkBeamerNotes->setChecked(this->params->getBeamerNotes());
    this->ui->radBeamerNotesRight->setChecked(this->params->getBeamerNotesPart() == BEAMER_NOTES_RIGHT);
    this->ui->radBeamerNotesLeft->setChecked(this->params->getBeamerNotesPart() == BEAMER_NOTES_LEFT);
    this->updateTextAnnot();

    QObject::connect(this->params, SIGNAL(textAnnotChanged()), SLOT(updateTextAnnot()));
}

OptionsDialog::~OptionsDialog()
{
    delete this->ui;
}

int OptionsDialog::getHours(int v)
{
    return v / (1000*60*60);
}

int OptionsDialog::getMinutes(int v)
{
    return (v % (1000*60*60)) / (1000*60);
}

int OptionsDialog::getSeconds(int v)
{
    return ((v % (1000*60*60)) % (1000*60)) / 1000;
}

void OptionsDialog::on_sliderSlidesWidth_valueChanged(int value)
{
    this->params->setCurrentSlidePrcentWidth(value / 100.0);
    this->ui->lblSlidesWidthValue->setText(QString("%1%").arg(value));
    this->ui->lblSlidesWidthValue->setAlignment(Qt::AlignRight);
    emit paramsChanged();
}

void OptionsDialog::on_buttonBox_accepted()
{
    QTime length = this->ui->timePresentationLength->time();
    QTime emergency = this->ui->timePresentationEmergency->time();

    this->params->setPresentationLength((length.minute() + length.hour()*60)*1000*60);
    this->params->setPresentationEmergency((emergency.minute() + emergency.hour()*60)*1000*60);

    this->params->setMainScreenId(this->ui->mainScreenIdentifier->text().toInt());
    this->params->setProjectorScreenId(this->ui->projectorScreenIdentifier->text().toInt());

    this->params->setBeamerNotes(this->ui->chkBeamerNotes->isChecked());
    this->params->setBeamerNotesPart(
                this->ui->radBeamerNotesRight->isChecked() ? BEAMER_NOTES_RIGHT : BEAMER_NOTES_LEFT
    );

    emit paramsChanged();
    emit resetPresentationCounter();
}

void OptionsDialog::on_buttonBox_rejected()
{
    this->params->setCurrentSlidePrcentWidth(this->oldCurrentSlidePrcentWidth);
    this->params->setPresentationEmergency(this->oldPresentationEmergency);
    this->params->setPresentationLength(this->oldPresentationLength);
    this->params->setProjectorScreenId(this->oldProjectorScreenId);
    this->params->setMainScreenId(this->oldMainScreenId);
    this->params->setBeamerNotes(this->oldBeamerNotes);
    this->params->setBeamerNotesPart(this->oldBeamerNotesPart);
    this->params->setTextAnnot(this->oldTextAnnot, false);

    emit paramsChanged();
}

void OptionsDialog::on_chkBeamerNotes_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        this->ui->radBeamerNotesRight->setEnabled(true);
        this->ui->radBeamerNotesLeft->setEnabled(true);
    }
    if (arg1 == Qt::Unchecked) {
        this->ui->radBeamerNotesRight->setEnabled(false);
        this->ui->radBeamerNotesLeft->setEnabled(false);
    }
}

void OptionsDialog::on_chkTextAnnot_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked) {
        this->params->setTextAnnot(true);
    }
    if (arg1 == Qt::Unchecked) {
        this->params->setTextAnnot(false);
    }

    emit paramsChanged();
}

void OptionsDialog::updateTextAnnot() {
    this->ui->chkTextAnnot->setChecked(this->params->getTextAnnot());
}

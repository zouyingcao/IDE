#include "encodingdialog.h"
#include "ui_encodingdialog.h"

encodingDialog::encodingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::encodingDialog)
{
    ui->setupUi(this);
}

encodingDialog::~encodingDialog()
{
    delete ui;
}

void encodingDialog::on_buttonBox_accepted()
{
    emit sendEncodingSignal(ui->utf_8Button->isChecked());
}

void encodingDialog::on_buttonBox_rejected()
{
    close();
}


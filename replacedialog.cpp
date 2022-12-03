#include "replacedialog.h"
#include "ui_replacedialog.h"

replaceDialog::replaceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::replaceDialog)
{
    ui->setupUi(this);
}

replaceDialog::~replaceDialog()
{
    delete ui;
}

void replaceDialog::on_replaceButton_clicked()
{
    QString target = ui->findEdit->text();
    QString to = ui->replaceEdit->text();
    emit sendReplaceSignal(target,to,ui->caseCheckBox->isChecked(),false);
    on_findButton_clicked();
}


void replaceDialog::on_replaceAllButton_clicked()
{
    QString target = ui->findEdit->text();
    QString to = ui->replaceEdit->text();
    emit sendReplaceSignal(target,to,ui->caseCheckBox->isChecked(),true);
}


void replaceDialog::on_closeButton_clicked()
{
    close();
}


void replaceDialog::on_findButton_clicked()
{
    QString target = ui->findEdit->text();
    emit sendFindSignal(target,ui->caseCheckBox->isChecked(),ui->backwardCheckBox->isChecked(),ui->wholeCheckBox->isChecked());
}


void replaceDialog::on_findEdit_textChanged(const QString &text)
{
    ui->findButton->setEnabled(!text.isEmpty());
}


void replaceDialog::on_replaceEdit_textChanged(const QString &text)
{
    ui->replaceButton->setEnabled(!text.isEmpty());
    ui->replaceAllButton->setEnabled(!text.isEmpty());
}


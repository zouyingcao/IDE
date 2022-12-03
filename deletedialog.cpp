#include "deletedialog.h"
#include "ui_deletedialog.h"

deleteDialog::deleteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::deleteDialog)
{
    ui->setupUi(this);
}

deleteDialog::~deleteDialog()
{
    delete ui;
}

void deleteDialog::on_cancelButton_clicked()
{
    close();
}

void deleteDialog::showText(QString str)
{
    ui->label->setText("你想要将更改保存到 "+str+" 吗");
}

void deleteDialog::on_saveButton_clicked()
{
    emit sendSaveSignal();
}


void deleteDialog::on_discardButton_clicked()
{
    emit sendDiscardSignal();
}


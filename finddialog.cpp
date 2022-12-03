#include "finddialog.h"
#include "ui_finddialog.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

findDialog::findDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::findDialog)
{
    ui->setupUi(this);
}

findDialog::~findDialog()
{
    delete ui;
}

void findDialog::on_findButton_clicked()
{
    QString text = ui->lineEdit->text();
    //输入的大小写是否敏感,向前还是向后查找
    emit sendFindSignal(text,ui->caseCheckBox->isChecked(),ui->backwardCheckBox->isChecked(),ui->wholeCheckBox->isChecked());
}


void findDialog::on_lineEdit_textChanged(const QString &text)
{
    ui->findButton->setEnabled(!text.isEmpty());
}


void findDialog::on_closeButton_clicked()
{
    close();
}


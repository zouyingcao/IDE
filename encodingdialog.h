#ifndef ENCODINGDIALOG_H
#define ENCODINGDIALOG_H

#include <QDialog>

namespace Ui {
class encodingDialog;
}

class encodingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit encodingDialog(QWidget *parent = nullptr);
    ~encodingDialog();

signals:
    void sendEncodingSignal(bool);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::encodingDialog *ui;
};

#endif // ENCODINGDIALOG_H

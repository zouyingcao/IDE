#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>

namespace Ui {
class findDialog;
}

class findDialog : public QDialog
{
    Q_OBJECT

public:
    explicit findDialog(QWidget *parent = nullptr);
    ~findDialog();

signals:
    void sendFindSignal(QString,bool,bool,bool);

private slots:
    void on_findButton_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

    void on_closeButton_clicked();

private:
    Ui::findDialog *ui;
};

#endif // FINDDIALOG_H

#ifndef REPLACEDIALOG_H
#define REPLACEDIALOG_H

#include <QDialog>

namespace Ui {
class replaceDialog;
}

class replaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit replaceDialog(QWidget *parent = nullptr);
    ~replaceDialog();

signals:
    void sendReplaceSignal(QString,QString,bool,bool);
    void sendFindSignal(QString,bool,bool,bool);

private slots:
    void on_replaceButton_clicked();

    void on_replaceAllButton_clicked();

    void on_closeButton_clicked();

    void on_findButton_clicked();

    void on_findEdit_textChanged(const QString &arg1);

    void on_replaceEdit_textChanged(const QString &arg1);

private:
    Ui::replaceDialog *ui;
};

#endif // REPLACEDIALOG_H

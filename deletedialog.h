#ifndef DELETEDIALOG_H
#define DELETEDIALOG_H

#include <QDialog>

namespace Ui {
class deleteDialog;
}

class deleteDialog : public QDialog
{
    Q_OBJECT

public:
    explicit deleteDialog(QWidget *parent = nullptr);
    void showText(QString);
    ~deleteDialog();

signals:
    void sendSaveSignal();
    void sendDiscardSignal();

private slots:
    void on_cancelButton_clicked();

    void on_saveButton_clicked();

    void on_discardButton_clicked();

private:
    Ui::deleteDialog *ui;
};

#endif // DELETEDIALOG_H

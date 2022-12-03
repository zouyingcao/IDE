#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <codeeditor.h>
#include <encodingdialog.h>
#include <finddialog.h>
#include <replacedialog.h>
#include <deletedialog.h>
#include <syntaxhighlighter.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showEncoding();
    void changeEncoding(bool);
    void setIconsEnable();
    void setContentEnable();
    void doSaveBeforeDelete();
    void doDiscard();
    void doFind(QString,bool,bool,bool);
    void doReplace(QString,QString,bool,bool);
    void on_newFile_triggered();
    void on_openFile_triggered();
    void on_encoding_triggered();
    void on_save_triggered();
    void on_saveAs_triggered();
    void on_undo_triggered();
    void on_copy_triggered();
    void on_cut_triggered();
    void on_paste_triggered();
    void on_compile_triggered();
    void on_exit_triggered();
    void on_redo_triggered();
    void on_quickFind_triggered();
    void on_quickReplace_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_tabWidget_currentChanged(int index);

    void on_run_triggered();

    void on_syntaxCheck_triggered();

    void on_CandR_triggered();

    void on_stop_triggered();

private:
    Ui::MainWindow *ui;
    //QTextEdit *currentTextEdit;
    CodeEditor *currentTextEdit;
    syntaxHighlighter *highlighter;
    QString fileName;
    QString filePath;
    encodingDialog *encodingDlg;
    findDialog *findDlg;
    replaceDialog *replaceDlg;
    deleteDialog *deleteDlg;
    int deleteIndex;
};
#endif // MAINWINDOW_H

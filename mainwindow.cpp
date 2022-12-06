#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "codeeditor.h"
#include <QtWidgets>
#include <QtDebug>
#include <QTextCodec>

#define cout qDebug()

QTextCodec *codec;

enum encoding{
    utf_8, gbk
};
enum encoding encoding;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //this->setWindowTitle(tr("欢迎来到zouyingcao的IDE"));
    this->setWindowTitle(tr("%1[*]").arg("欢迎来到zouyingcao的IDE"));

    //gbk->utf-8:codec->toUnicode()
    //utf-8->gbk:codec->fromUnicode();
    codec = QTextCodec::codecForName("UTF-8");

    //IDE的编码默认是UTF-8
    encoding = utf_8;
    showEncoding();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::documentWasModified()
{
    setWindowModified(currentTextEdit->document()->isModified());
}

//展示编码方式
void MainWindow::showEncoding()
{
    ui->label->setText(codec->toUnicode(encoding==utf_8? "编码格式:UTF-8":"编码格式:GBK"));
}

//改变编码方式
void MainWindow::changeEncoding(bool flag)
{
    if(flag){
        encoding = utf_8;
    }else{
        encoding = gbk;
    }
    showEncoding();
}

void MainWindow::setIconsEnable()
{
    ui->save->setEnabled(ui->tabWidget->count());//保存
    ui->saveAs->setEnabled(ui->tabWidget->count());//另存为
    ui->paste->setEnabled(ui->tabWidget->count());//粘贴
    ui->quickFind->setEnabled(ui->tabWidget->count());//查找
    ui->quickReplace->setEnabled(ui->tabWidget->count());//替换
    ui->compile->setEnabled(ui->tabWidget->count());//编译
    ui->run->setEnabled(ui->tabWidget->count());//运行
    ui->CandR->setEnabled(ui->tabWidget->count());//编译运行
    ui->syntaxCheck->setEnabled(ui->tabWidget->count());//语法检查
}

void MainWindow::setContentEnable()
{
    if(currentTextEdit!=NULL){
        //connect(currentTextEdit,&QTextEdit::redoAvailable,ui->redo,&QAction::setEnabled);
        //connect(currentTextEdit,&QTextEdit::undoAvailable,ui->undo,&QAction::setEnabled);
        //connect(currentTextEdit, &QTextEdit::copyAvailable, ui->cut, &QAction::setEnabled);
        //connect(currentTextEdit, &QTextEdit::copyAvailable, ui->copy, &QAction::setEnabled);
        connect(currentTextEdit,&QPlainTextEdit::redoAvailable,ui->redo,&QAction::setEnabled);
        connect(currentTextEdit,&QPlainTextEdit::undoAvailable,ui->undo,&QAction::setEnabled);
        connect(currentTextEdit, &QPlainTextEdit::copyAvailable, ui->cut, &QAction::setEnabled);
        connect(currentTextEdit, &QPlainTextEdit::copyAvailable, ui->copy, &QAction::setEnabled);
        //当前内容改变
        connect(currentTextEdit->document(),&QTextDocument::contentsChanged, this, &MainWindow::documentWasModified);
    }
}

//新建
void MainWindow::on_newFile_triggered()
{
    auto page = new QWidget();
    fileName = "源"+(ui->tabWidget->count()==0?"":QString::number(ui->tabWidget->count()))+".cpp";
    page->setAttribute(Qt::WA_DeleteOnClose);//关闭时自动销毁
    page->setToolTip("");//放保存路径,新建时为空
    page->setLayout(new QGridLayout);
    //QTextEdit *TextEdit = new QTextEdit();
    CodeEditor *TextEdit = new CodeEditor();
    TextEdit->setAttribute(Qt::WA_DeleteOnClose);
    TextEdit->setObjectName("textEdit");
    page->layout()->addWidget(TextEdit);
    const int index = ui->tabWidget->addTab(page,fileName);//tab名称为源xx.cpp
    ui->tabWidget->setCurrentIndex(index);
    currentTextEdit = TextEdit;
    filePath = page->toolTip();
    setIconsEnable();
    setContentEnable();
    //语法高亮
    highlighter = new syntaxHighlighter(currentTextEdit->document());
}

//打开
void MainWindow::on_openFile_triggered()
{
    QString path = QFileDialog::getOpenFileName();

    if(path.isEmpty()){
        cout<<"filePath is empty";
        return;
    }else{
        QFile file(path);
        if (file.open(QFile::ReadOnly | QFile::Text)){
            on_newFile_triggered();//新建一个页面放打开的文件
            currentTextEdit->setPlainText(file.readAll()); // setText()设置内容, toPlainText()获取内容
            filePath = path;
            ui->tabWidget->currentWidget()->setToolTip(path);// 放打开文件的路径
//            QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h|(?<=\\/)\\w+\\.txt"));
//            fileName=re.match(filePath).captured();//捕捉文件名
            int cnt=filePath.length();
            int i=filePath.lastIndexOf("/");
            fileName=filePath.right(cnt-i-1);
            this->setWindowTitle(tr("%1[*]").arg("IDE - "+fileName));
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),fileName);
            setWindowModified(false);
            setIconsEnable();
            setContentEnable();
        }
        else{
            cout<<"on_openFile_triggered fopen err";
            return;
        }
    }
}

//编码方式
void MainWindow::on_encoding_triggered()
{
    encodingDlg = new encodingDialog(this);
    encodingDlg->setModal(true);
    encodingDlg->show();
    connect(encodingDlg, SIGNAL(sendEncodingSignal(bool)), this, SLOT(changeEncoding(bool)));
}

//保存
void MainWindow::on_save_triggered()
{
    if(filePath.isEmpty()){
        on_saveAs_triggered();
    }else{
        QFile out(filePath);
        if(out.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream str(&out);
            str<<currentTextEdit->toPlainText();
            out.close();
            setWindowModified(false);
        }else{
            cout<<"on_save_triggered fopen err";
            return;
        }
    }
}

//另存为
void MainWindow::on_saveAs_triggered()
{
    QString savePath = QFileDialog::getSaveFileName(this,"",fileName,tr("Cpp File(*.cpp *.c *.h)"));

    if(savePath.isEmpty()){
        cout<<"savePath is empty";
        return;
    }else{
        QFile out(savePath);
        if(out.open(QIODevice::WriteOnly|QIODevice::Text)){
            QTextStream str(&out);
            str<<currentTextEdit->toPlainText();
            out.close();

            //QRegularExpression re(tr("(?<=\\/)\\w+\\.cpp|(?<=\\/)\\w+\\.c|(?<=\\/)\\w+\\.h|(?<=\\/)\\w+\\.txt"));
            //fileName=re.match(savePath).captured();
            int cnt=savePath.length();
            int i=savePath.lastIndexOf("/");
            fileName=savePath.right(cnt-i-1);
            filePath=savePath;
            ui->tabWidget->currentWidget()->setToolTip(savePath);// 放文件另存为的路径
            ui->tabWidget->setTabText(ui->tabWidget->currentIndex(),fileName);
            this->setWindowTitle(tr("%1[*]").arg("IDE - "+fileName));
            setWindowModified(false);
        }else{
            cout<<"on_saveFile_triggered fopen err";
            return;
        }
    }
}

//撤销
void MainWindow::on_undo_triggered()
{
    currentTextEdit->undo();
}

//重做
void MainWindow::on_redo_triggered()
{
    currentTextEdit->redo();
}

//复制
void MainWindow::on_copy_triggered()
{
    currentTextEdit->copy();
}

//剪切
void MainWindow::on_cut_triggered()
{
    currentTextEdit->cut();
}

//粘贴
void MainWindow::on_paste_triggered()
{
    currentTextEdit->paste();
}

//退出
void MainWindow::on_exit_triggered()
{
    exit(0);
}

//快速查找
void MainWindow::on_quickFind_triggered()
{
    findDlg = new findDialog(this);
    findDlg->setModal(true);
    findDlg->show();
    connect(findDlg, SIGNAL(sendFindSignal(QString,bool,bool,bool)), this, SLOT(doFind(QString,bool,bool,bool)));
}

void MainWindow::doFind(QString str, bool findCaseSensitively, bool findBackward, bool findWholeWords)
{
    // FindFlags:可以看到该参数是一个枚举变量，用来指定查找的方式，分别是向后查找、区分大小写、全词匹配等。
    // 如果不指定该参数，默认的是向前查找、不区分大小写、包含该字符串的词也可以查找到。这几个变量还可以使用“|”符号来一起使用。
    // enum FindFlag{FindBackward = 0x00001, FindCaseSensitively = 0x00002, FindWholeWords = 0x00004};

    bool find = true;
    if (findBackward&&findCaseSensitively&&findWholeWords){//向后查找、区分大小写、全词匹配
        find = currentTextEdit->find(str, QTextDocument::FindBackward|QTextDocument::FindCaseSensitively|QTextDocument::FindWholeWords);
    }else if(findBackward&&findCaseSensitively){//向后查找、区分大小写
        find = currentTextEdit->find(str, QTextDocument::FindBackward|QTextDocument::FindCaseSensitively);
    }else if(findBackward&&findWholeWords){//向后查找、全词匹配
        find = currentTextEdit->find(str, QTextDocument::FindBackward|QTextDocument::FindWholeWords);
    }else if(findCaseSensitively&&findWholeWords){//区分大小写、全词匹配
        find = currentTextEdit->find(str, QTextDocument::FindCaseSensitively|QTextDocument::FindWholeWords);
    }else if(findBackward){//向后查找
        find = currentTextEdit->find(str, QTextDocument::FindBackward);
    }else if(findCaseSensitively){//区分大小写
        find = currentTextEdit->find(str, QTextDocument::FindCaseSensitively);
    }else if(findWholeWords){//全词匹配
        find = currentTextEdit->find(str, QTextDocument::FindWholeWords);
    }else {//默认
        find = currentTextEdit->find(str);
    }

    if(!find){
        QMessageBox::warning(this, tr("lookup"),
        tr("Not found A %1").arg(str));
    }
}

//快速替换
void MainWindow::on_quickReplace_triggered()
{
    replaceDlg = new replaceDialog(this);
    replaceDlg->setModal(true);
    replaceDlg->show();
    connect(replaceDlg, SIGNAL(sendReplaceSignal(QString,QString,bool,bool)), this, SLOT(doReplace(QString,QString,bool,bool)));
    connect(replaceDlg, SIGNAL(sendFindSignal(QString,bool,bool,bool)), this, SLOT(doFind(QString,bool,bool,bool)));
}

void MainWindow::doReplace(QString target, QString to, bool sensitive, bool all)
{
    if(all)
    {
        QString text = currentTextEdit->toPlainText();
        text.replace(target, to, sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive);
        currentTextEdit->clear();
        currentTextEdit->insertPlainText(text);
    }else{
        QString selectedText =  currentTextEdit->textCursor().selectedText();
        if(selectedText == target)
            currentTextEdit->insertPlainText(to);
        else if(!sensitive&&selectedText !="")
            currentTextEdit->insertPlainText(to);
    }
    setContentEnable();
}


void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    deleteIndex = index;
    deleteDlg = new deleteDialog(this);//弹出询问是否保存
    deleteDlg->setModal(true);
    deleteDlg->showText(ui->tabWidget->tabToolTip(index)==""? ui->tabWidget->tabText(index):ui->tabWidget->tabToolTip(index));
    deleteDlg->show();
    connect(deleteDlg, SIGNAL(sendSaveSignal()), this, SLOT(doSaveBeforeDelete()));
    connect(deleteDlg, SIGNAL(sendDiscardSignal()), this, SLOT(doDiscard()));
}

void MainWindow::doDiscard()
{
    ui->tabWidget->removeTab(deleteIndex);
    deleteDlg->close();
    setIconsEnable();
    setContentEnable();
}

void MainWindow::doSaveBeforeDelete()
{
    on_save_triggered();
    ui->tabWidget->removeTab(deleteIndex);
    deleteDlg->close();
    setIconsEnable();
    setContentEnable();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //currentTextEdit = ui->tabWidget->currentWidget()->findChild<QTextEdit *>("textEdit");
    currentTextEdit = ui->tabWidget->currentWidget()->findChild<CodeEditor *>("textEdit");
    setContentEnable();
    fileName = ui->tabWidget->tabText(index);
    filePath = ui->tabWidget->tabToolTip(index);
    ui->tabWidget->setCurrentIndex(index);
}

//编译
void MainWindow::on_compile_triggered()
{
    if(filePath.isEmpty()){
        //调出保存窗口
        on_save_triggered();
    }
    //确定保存了的情况后
    if(!filePath.isEmpty()){
        if(isWindowModified()){
            int r=QMessageBox::warning(this, tr("提示"),
                                       tr("the text has been modified.\nDo you want to save it ?"),
                                       QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(r==QMessageBox::Cancel)
                return;//无动作退出
            else if(r==QMessageBox::Yes)
                on_save_triggered();
            else if(r==QMessageBox::No)
                ui->textBrowser_log->append("已启动编译之前的文件版本\n");
        }
        QProcess p(0);
        QString command ="D:\\QtProject\\Compiler.exe";//编译的程序
        p.setProgram(command);
        QStringList args;
        args<<filePath;
        p.setArguments(args);
        p.start();
        p.waitForStarted(); //等待程序启动
        ui->stop->setEnabled(true);
        p.waitForFinished();//等待程序关闭
        ui->stop->setEnabled(false);
        ui->textBrowser_log->setText("正在编译 "+filePath+"\n");
        QString output=QString::fromLocal8Bit(p.readAllStandardOutput()); //程序输出信息
        ui->textBrowser_asm->setText(output);

        QFile markFile(fileName+"Mark.txt");
        if(markFile.exists()){
            ui->textBrowser_log->append("之前已编译过该文件,现覆盖结果");
        }else{
            ui->textBrowser_log->append("第一次编译该文件,正在生成结果");
            markFile.open(QIODevice::ReadWrite|QIODevice::Text);
            markFile.close();
        }
        // 错误信息
        if(output.startsWith("error")||output.startsWith("Error"))
            ui->textBrowser_log->append(output);//程序输出信息以error/Error开头
        ui->textBrowser_log->append(QString::fromLocal8Bit(p.readAllStandardError())); // cerr的输出信息
    }
}

//运行
void MainWindow::on_run_triggered()
{
    if(filePath.isEmpty()){
        ui->textBrowser_log->setText("请先保存并编译当前文件!\n");
    }
    //确定保存了的情况后
    else {
        QFile markFile(fileName+"Mark.txt");
        // 未编译过的情况
        if(!markFile.exists()){
            int r=QMessageBox::warning(this, tr("提示"),
                               tr("the text has not been compiled.\nDo you want to compile now ?"),
                               QMessageBox::Yes|QMessageBox::No);
            if(r==QMessageBox::No)
                return;//无动作退出
            else if(r==QMessageBox::Yes)
                on_compile_triggered();
        }
        if(isWindowModified()){
            int r=QMessageBox::warning(this, tr("提示"),
                               tr("the text has been modified.\nDo you want to save it ?"),
                               QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(r==QMessageBox::Cancel)
                return;//无动作退出
            else if(r==QMessageBox::Yes){
                on_save_triggered();
                on_compile_triggered();
            }
            else if(r==QMessageBox::No)
                ui->textBrowser_log->append("已启动运行之前的文件版本\n");
        }
        // 确定了文件保存为最新版本的情况
        ui->textBrowser_log->append("正在运行 "+filePath+"\n");

        QProcess p(0);
        QString command ="D:\\QtProject\\minsys_asm.exe";//汇编器程序
        p.setProgram(command);
        QStringList args;
        args<<"ObjectCode.txt";
        //args<<"testinterface.asm";
        p.setArguments(args);
        p.start();
        p.waitForStarted(); //等待程序启动
        ui->stop->setEnabled(true);
        p.waitForFinished();//等待程序关闭
        ui->stop->setEnabled(false);
        ui->textBrowser_log->setText("正在运行 "+filePath+"，生成COE文件中...\n");
        //log文件
        QString errordata;
        QFile file("D:\\QtProject\\asmError.log");
        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<<"asmError.log文件未打开!";
        while(!file.atEnd()){
            QByteArray array = file.readLine();
            QString str(array);
            errordata.append(str);
        }
        ui->textBrowser_log->append(errordata);
        ui->textBrowser_log->append(QString::fromLocal8Bit(p.readAllStandardError())); // cerr的输出信息

        //程序输出信息coe文件
        QString data;
        QFile coefile("D:\\QtProject\\prgmip32.coe");
        if(!coefile.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<<"prgmip32.coe文件未打开!";
        while(!coefile.atEnd()){
            QByteArray array = coefile.readLine();
            QString str(array);
            data.append(str);
        }
        ui->textBrowser_coe->setText("prgmip32.coe文件内容如下：\n");
        ui->textBrowser_coe->append(data);

        data.clear();
        QFile coefile1("D:\\QtProject\\dmem32.coe");
        if(!coefile1.open(QIODevice::ReadOnly | QIODevice::Text))
            qDebug()<<"prgmip32.coe文件未打开!";
        while(!coefile1.atEnd()){
            QByteArray array = coefile1.readLine();
            QString str(array);
            data.append(str);
        }
        ui->textBrowser_coe->append("\ndmem32.coe文件内容如下：\n");
        ui->textBrowser_coe->append(data);
    }
}

//语法检查
void MainWindow::on_syntaxCheck_triggered()
{
    if(filePath.isEmpty()){
        //调出保存窗口
        on_save_triggered();
    }
    //确定保存了的情况后
    if(!filePath.isEmpty()){
        if(isWindowModified()){
            int r=QMessageBox::warning(this, tr("提示"),
                                       tr("the text has been modified.\nDo you want to save it ?"),
                                       QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
            if(r==QMessageBox::Cancel)
                return;//无动作退出
            else if(r==QMessageBox::Yes)
                on_save_triggered();
            else if(r==QMessageBox::No)
                ui->textBrowser_log->append("对之前的文件版本进行语法检查\n");
        }
        QProcess p(0);
        QString command ="D:\\QtProject\\CompilerCheck.exe";//语法检查的程序
        p.setProgram(command);
        QStringList args;
        args<<filePath;
        p.setArguments(args);
        p.start();
        p.waitForStarted(); //等待程序启动
        p.waitForFinished();//等待程序关闭
        ui->textBrowser_log->setText("正在语法检查 "+filePath+"\n");
        ui->textBrowser_asm->setText(QString::fromLocal8Bit(p.readAllStandardOutput())); //cout的内容
        ui->textBrowser_log->append(QString::fromLocal8Bit(p.readAllStandardError())); // cerr的输出信息
    }
}

//编译运行
void MainWindow::on_CandR_triggered()
{
    on_compile_triggered();
    on_run_triggered();
}

//停止运行
void MainWindow::on_stop_triggered()
{

}


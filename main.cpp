#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //对于按钮文字显示不全的解决方法，必须放在main中的第一行
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    codeeditor.cpp \
    deletedialog.cpp \
    encodingdialog.cpp \
    finddialog.cpp \
    main.cpp \
    mainwindow.cpp \
    replacedialog.cpp \
    syntaxhighlighter.cpp

HEADERS += \
    codeeditor.h \
    deletedialog.h \
    encodingdialog.h \
    finddialog.h \
    mainwindow.h \
    replacedialog.h \
    syntaxhighlighter.h

FORMS += \
    deletedialog.ui \
    encodingdialog.ui \
    finddialog.ui \
    mainwindow.ui \
    replacedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc

RC_ICONS += ide.ico

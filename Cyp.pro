#-------------------------------------------------
#
# Project created by QtCreator 2016-10-08T14:09:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Cyp
TEMPLATE = app


SOURCES += main.cpp\
        cryptarithmetic.cpp \
    digit.cpp \
    equation.cpp \
    editdialog.cpp \
    pooldialog.cpp \
    api/app.cpp \
    api/appinit.cpp \
    api/iconhelper.cpp \
    usercontrol/frminputbox.cpp \
    usercontrol/frmmessagebox.cpp \
    resultdialog.cpp \
    gendialog.cpp \
    waitingdialog.cpp

HEADERS  += cryptarithmetic.h \
    digit.h \
    equation.h \
    editdialog.h \
    pooldialog.h \
    api/app.h \
    api/appinit.h \
    api/iconhelper.h \
    api/myhelper.h \
    usercontrol/frminputbox.h \
    usercontrol/frmmessagebox.h \
    resultdialog.h \
    gendialog.h \
    waitingdialog.h

FORMS    += cryptarithmetic.ui \
    editdialog.ui \
    pooldialog.ui \
    usercontrol/frminputbox.ui \
    usercontrol/frmmessagebox.ui \
    resultdialog.ui \
    gendialog.ui \
    waitingdialog.ui

RESOURCES += \
    other/rc.qrc

DISTFILES +=

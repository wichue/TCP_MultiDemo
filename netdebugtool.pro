#-------------------------------------------------
#
# Project created by QtCreator 2020-04-03T15:09:03
#
#-------------------------------------------------

QT       += core gui network

RC_ICONS = tcplogo.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = netdebugtool
TEMPLATE = app

#DESTDIR = ./bin


SOURCES += main.cpp\
        mymainwindow.cpp \
    server.cpp \
    tcpserverwin.cpp \
    mywidget.cpp \
    tcpconnctiondialog.cpp \
    newserverdialog.cpp

HEADERS  += mymainwindow.h \
    server.h \
    tcpserverwin.h \
    mywidget.h \
    header.h \
    tcpconnctiondialog.h \
    newserverdialog.h

FORMS    += mymainwindow.ui \
    tcpserverwin.ui \
    mywidget.ui \
    tcpconnctiondialog.ui \
    newserverdialog.ui
CONFIG += c++11

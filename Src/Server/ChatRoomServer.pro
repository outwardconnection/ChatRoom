#-------------------------------------------------
#
# Project created by QtCreator 2016-07-20T23:06:59
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatRoomServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    chatroomserver.cpp \
    sleepthread.cpp

HEADERS  += mainwindow.h \
    internetserver.h \
    chatroomserver.h \
    sleepthread.h

FORMS    += mainwindow.ui

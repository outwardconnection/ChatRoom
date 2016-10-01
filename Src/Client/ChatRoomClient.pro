#-------------------------------------------------
#
# Project created by QtCreator 2016-07-17T12:01:21
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ChatRoomClient
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settinginterface.cpp \
    sleepthread.cpp \
    relayform.cpp \
    serverlistitem.cpp \
    scrollareamodify.cpp

HEADERS  += mainwindow.h \
    settinginterface.h \
    sleepthread.h \
    relayform.h \
    ServerListItem.h \
    scrollareamodify.h

FORMS    += mainwindow.ui

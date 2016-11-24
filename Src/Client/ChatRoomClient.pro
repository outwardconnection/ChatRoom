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
    settinginterface.cpp \
    sleepthread.cpp \
    relayform.cpp \
    serverlistitem.cpp \
    scrollareamodify.cpp \
    clientcontrol.cpp \
    chatroomclient.cpp

HEADERS  += \
    settinginterface.h \
    sleepthread.h \
    relayform.h \
    ServerListItem.h \
    scrollareamodify.h \
    clientcontrol.h \
    chatroomclient.h

FORMS    += \
    chatroomclient.ui

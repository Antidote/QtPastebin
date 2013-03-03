#-------------------------------------------------
#
# Project created by QtCreator 2013-02-13T13:00:45
#
#-------------------------------------------------

QT       += core network

QT       -= gui

TARGET = PastebinAPI
TEMPLATE = lib
CONFIG += staticlib

DEFINES += PASTEBIN_LIBRARY

SOURCES += pastebin.cpp \
    pasteinfo.cpp \
    userinfo.cpp

HEADERS += pastebin.h\
    pasteinfo.h \
    userinfo.h \
    PastebinAPI.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

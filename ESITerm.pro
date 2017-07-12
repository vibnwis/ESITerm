#-------------------------------------------------
#
# Project created by QtCreator 2016-11-14T00:22:30
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ESITerm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    esisettings.cpp \
    esitimer.cpp

HEADERS  += mainwindow.h \
    esisettings.h \
    esitimer.h

FORMS    += mainwindow.ui \
    esisettings.ui

RESOURCES += \
    esiresources.qrc

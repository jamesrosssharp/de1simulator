#-------------------------------------------------
#
# Project created by QtCreator 2016-05-15T13:15:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = de1simulator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    hexdisplaylabel.cpp \
    vgawindow.cpp

HEADERS  += mainwindow.h \
    sharedmem.h \
    hexdisplaylabel.h \
    vgawindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    de1resources.qrc

#-------------------------------------------------
#
# Project created by QtCreator 2017-06-26T11:24:42
#
#-------------------------------------------------

QT       += core gui
CONFIG   += c++11

CONFIG += c++11s

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VIPainter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vpoint.cpp \
    vpolygon.cpp \
    vellipse.cpp \
    vshape.cpp \
    vcurve.cpp \
    vgroupshape.cpp

HEADERS  += mainwindow.h \
    vpoint.h \
    vpolygon.h \
    vshape.h \
    vellipse.h \
    vcurve.h \
    vgroupshape.h

FORMS    += mainwindow.ui

RESOURCES += \
    icon.qrc

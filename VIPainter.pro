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
    vgroupshape.cpp \
    vpolyline.cpp \
    vsize.cpp \
    vtype.cpp \
    vcurveline.cpp \
    testwidget.cpp \
    interpolation.cpp \
    vpointgroupshape.cpp \
    vmagnification.cpp \
    vcursortype.cpp \
    vdocktitlebar.cpp \
    canvassizedialog.cpp \
    vvector.cpp \
    vbezlercurve.cpp \
    vtext.cpp \
    vtransform.cpp \
    changetextdialog.cpp \
    vroundedrectangle.cpp \
    vdelegate.cpp \
    vlistview.cpp

HEADERS  += mainwindow.h \
    vpoint.h \
    vpolygon.h \
    vshape.h \
    vellipse.h \
    vgroupshape.h \
    vpolyline.h \
    vsize.h \
    vtype.h \
    vcurveline.h \
    testwidget.h \
    interpolation.h \
    vpointgroupshape.h \
    vmagnification.h \
    vcursortype.h \
    vdocktitlebar.h \
    canvassizedialog.h \
    vvector.h \
    vbezlercurve.h \
    vtext.h \
    vtransform.h \
    changetextdialog.h \
    vroundedrectangle.h \
    vdelegate.h \
    vlistview.h

FORMS    += mainwindow.ui \
    canvassizedialog.ui \
    changetextdialog.ui

RESOURCES += \
    icon.qrc

RC_ICONS = icon/VIPainter.ico

#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T15:10:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ClientQtTcp
TEMPLATE = app
QT += widgets network
DEPENDPATH += .
INCLUDEPATH += .


SOURCES += main.cpp \
    clientwindow.cpp \
    controlpannel.cpp

HEADERS  += \
    clientwindow.h \
    controlpannel.h

FORMS    +=

#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T12:21:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JenkinsTray
TEMPLATE = app


SOURCES += main.cpp\
    trayicon.cpp \
    configuration.cpp \
    configurationwindow.cpp \
    backend.cpp

HEADERS  += \
    trayicon.h \
    configuration.h \
    configurationwindow.h \
    backend.h

RESOURCES += \
    icons.qrc

FORMS += \
    ConfigurationWindow.ui

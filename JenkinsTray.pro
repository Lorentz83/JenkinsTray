#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T12:21:13
#
#-------------------------------------------------

QT       += core gui network xml xmlpatterns

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JenkinsTray
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp\
    trayicon.cpp \
    configuration.cpp \
    configurationwindow.cpp \
    backend.cpp \
    jenkinsjob.cpp

HEADERS  += \
    trayicon.h \
    configuration.h \
    configurationwindow.h \
    backend.h \
    jenkinsjob.h

RESOURCES += \
    icons.qrc

FORMS += \
    ConfigurationWindow.ui

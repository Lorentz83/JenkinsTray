#-------------------------------------------------
#
# Project created by QtCreator 2015-09-04T12:21:13
#
#-------------------------------------------------

QT       += core gui widgets network xml xmlpatterns multimedia

QMAKE_CXXFLAGS += -std=c++11

TARGET = JenkinsTray
TEMPLATE = app

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
    res.qrc

FORMS += \
    ConfigurationWindow.ui


release: DESTDIR = ../build/release
debug:   DESTDIR = ../build/debug
OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

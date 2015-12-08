#-------------------------------------------------
#
# Project created by QtCreator 2015-12-07T14:09:38
#
#-------------------------------------------------

QT       += core gui widgets network xml xmlpatterns multimedia
QT       += testlib

QMAKE_CXXFLAGS += -std=c++11

TARGET = test
TEMPLATE = app

SOURCES += \
    ../src/jenkinsjob.cpp \
    testall.cpp \
    jobstatustest.cpp \
    ../src/backend.cpp \
    parsetest.cpp \
    ../src/configuration.cpp

HEADERS += \
    ../src/jenkinsjob.h \
    jobstatustest.h \
    ../src/backend.h \
    parsetest.h \
    ../src/configuration.h

DISTFILES +=

RESOURCES += \
    rss.qrc


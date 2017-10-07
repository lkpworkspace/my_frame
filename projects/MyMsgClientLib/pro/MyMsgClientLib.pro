TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../inc

SOURCES += \
    ../src/main.cpp \
    ../src/MyMsg.cpp

HEADERS += \
    ../inc/MyMsg.h

#-------------------------------------------------
#
# Project created by QtCreator 2017-06-21T04:20:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CapClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# add by myself
INCLUDEPATH += ../../../
LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector

FORMS    += MainWidget.ui

SOURCES += main.cpp\
    ../../../Common.cpp \
    ../../../MySock.cpp \
    ../../../MyNet.cpp \
    ../../../MyThread.cpp \
    ../../../MyEvent.cpp \
    ../../../MyApp.cpp \
    ../../../MyLog.cpp \
    ../../../MyHelp.cpp \
    ../../../MyTask.cpp \
    ../../../MyList.cpp \
    ../../../MyUdp.cpp \
    ../../../MyTcp.cpp \
    ../../../MyBase64.cpp \
    ../../../MyMouseEvent.cpp \
    ../../../MyAllEvent.cpp \
    ../../../3rd_src/Sqlite3/sqlite3.c \
    ../../../MySqlite3.cpp \
    ../../../MyTFTP.cpp \
    ../../../MyKeyEvent.cpp \
    ../../../MyNormalEvent.cpp \
    ../../../MyTimer.cpp \
    ../../../MyControls.cpp \
    MainWidget.cpp \
    MyCapClient.cpp \
    MyLogin.cpp

HEADERS  += MainWidget.h \
    ../../../Common.h \
    ../../../MySock.h \
    ../../../MyNet.h \
    ../../../MyThread.h \
    ../../../MyEvent.h \
    ../../../MyApp.h \
    ../../../MyList.h \
    ../../../MyVec.h \
    ../../../MyLog.h \
    ../../../MyHelp.h \
    ../../../MyTask.h \
    ../../../MyUdp.h \
    ../../../MyTcp.h \
    ../../../MyBase64.h \
    ../../../MyHeap.h \
    ../../../MyHash.h \
    ../../../MyMouseEvent.h \
    ../../../MyAllEvent.h \
    ../../../MyFrame.h \
    ../../../3rd_src/Sqlite3/sqlite3.h \
    ../../../MySqlite3.h \
    ../../../MyTFTP.h \
    ../../../MyKeyEvent.h \
    ../../../3rd_src/Sqlite3/sqlite3ext.h \
    ../../../MyNormalEvent.h \
    ../../../MyTimer.h \
    ../../../MyControls.h \
    MainWidget.h \
    MyCapClient.h \
    MyLogin.h

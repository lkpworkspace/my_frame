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
INCLUDEPATH += ../../../../MyFrame/inc
INCLUDEPATH += ../../../../3rd_src
LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector

FORMS    += ../ui/MainWidget.ui

SOURCES += \
    ../../../../MyFrame/src/Common.cpp \
    ../../../../MyFrame/src/MyHelp.cpp \
    ../../../../MyFrame/src/MyLog.cpp \
    ../../../../MyFrame/src/MyApp.cpp \
    ../../../../MyFrame/src/MyThread.cpp \
    ../../../../MyFrame/src/MyEvent.cpp \
    ../../../../MyFrame/src/MyTask.cpp \
    ../../../../MyFrame/src/MyList.cpp \
    ../../../../MyFrame/src/MyTimer.cpp \
    ../../../../MyFrame/src/MyNet.cpp \
    ../../../../MyFrame/src/MySock.cpp \
    ../../../../MyFrame/src/MyUdp.cpp \
    ../../../../MyFrame/src/MyTcp.cpp \
    ../../../../MyFrame/src/MyBase64.cpp \
    ../../../../MyFrame/src/MyMouseEvent.cpp \
    ../../../../MyFrame/src/MyKeyEvent.cpp \
    ../../../../MyFrame/src/MyAllEvent.cpp \
    ../../../../MyFrame/src/MyTFTP.cpp \
    ../../../../MyFrame/src/MyNormalEvent.cpp \
    ../../../../MyFrame/src/MyControls.cpp \
    ../../../../MyFrame/src/MyTest.cpp \
    ../../../../MyFrame/src/MySqlite3.cpp \
    ../../../../3rd_src/Sqlite3/sqlite3.c \
    ../src/MainWidget.cpp \
    ../src/MyCapClient.cpp \
    ../src/MyLogin.cpp \
    ../src/main.cpp

HEADERS  += \
    ../../../../MyFrame/inc/Common.h \
    ../../../../MyFrame/inc/MyHelp.h \
    ../../../../MyFrame/inc/MyLog.h \
    ../../../../MyFrame/inc/MyApp.h \
    ../../../../MyFrame/inc/MyList.h \
    ../../../../MyFrame/inc/MyThread.h \
    ../../../../MyFrame/inc/MyTask.h \
    ../../../../MyFrame/inc/MyEvent.h \
    ../../../../MyFrame/inc/MyControls.h \
    ../../../../MyFrame/inc/MyTimer.h \
    ../../../../MyFrame/inc/MyNet.h \
    ../../../../MyFrame/inc/MySock.h \
    ../../../../MyFrame/inc/MyUdp.h \
    ../../../../MyFrame/inc/MyTcp.h \
    ../../../../MyFrame/inc/MyVec.h \
    ../../../../MyFrame/inc/MyBase64.h \
    ../../../../MyFrame/inc/MyHeap.h \
    ../../../../MyFrame/inc/MyHash.h \
    ../../../../MyFrame/inc/MyMouseEvent.h \
    ../../../../MyFrame/inc/MyKeyEvent.h \
    ../../../../MyFrame/inc/MyAllEvent.h \
    ../../../../MyFrame/inc/MyFrame.h \
    ../../../../MyFrame/inc/MyTFTP.h \
    ../../../../MyFrame/inc/MyNormalEvent.h \
    ../../../../MyFrame/inc/MyTest.h \
    ../../../../MyFrame/inc/MySqlite3.h \
    ../../../../3rd_src/Sqlite3/sqlite3.h \
    ../../../../3rd_src/Sqlite3/sqlite3ext.h \
    ../inc/MainWidget.h \
    ../inc/MyCapClient.h \
    ../inc/MyLogin.h

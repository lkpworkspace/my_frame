TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# add by myself
INCLUDEPATH += ../../../../MyFrame/inc
LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder

SOURCES += \
    ../../../../MyFrame/src/Common.cpp \
    ../../../../MyFrame/src/MySock.cpp \
    ../../../../MyFrame/src/MyNet.cpp \
    ../../../../MyFrame/src/MyThread.cpp \
    ../../../../MyFrame/src/MyEvent.cpp \
    ../../../../MyFrame/src/MyApp.cpp \
    ../../../../MyFrame/src/MyLog.cpp \
    ../../../../MyFrame/src/MyHelp.cpp \
    ../../../../MyFrame/src/MyTask.cpp \
    ../../../../MyFrame/src/MyList.cpp \
    ../../../../MyFrame/src/MyUdp.cpp \
    ../../../../MyFrame/src/MyTcp.cpp \
    ../../../../MyFrame/src/MyBase64.cpp \
    ../../../../MyFrame/src/MyMouseEvent.cpp \
    ../../../../MyFrame/src/MyAllEvent.cpp \
    ../../../../MyFrame/src/MySqlite3.cpp \
    ../../../../MyFrame/src/MyKeyEvent.cpp \
    ../../../../MyFrame/src/MyNormalEvent.cpp \
    ../../../../MyFrame/src/MyTimer.cpp \
    ../../../../MyFrame/src/MyControls.cpp \
    ../../../../MyFrame/src/MyTest.cpp \
    ../../../../3rd_src/Sqlite3/sqlite3.c \
    ../src/MyCapConnect.cpp \
    ../src/MyServer.cpp \
    ../src/main.cpp

HEADERS  += \
    ../../../../MyFrame/inc/Common.h \
    ../../../../MyFrame/inc/MySock.h \
    ../../../../MyFrame/inc/MyNet.h \
    ../../../../MyFrame/inc/MyThread.h \
    ../../../../MyFrame/inc/MyEvent.h \
    ../../../../MyFrame/inc/MyApp.h \
    ../../../../MyFrame/inc/MyList.h \
    ../../../../MyFrame/inc/MyVec.h \
    ../../../../MyFrame/inc/MyLog.h \
    ../../../../MyFrame/inc/MyHelp.h \
    ../../../../MyFrame/inc/MyTask.h \
    ../../../../MyFrame/inc/MyUdp.h \
    ../../../../MyFrame/inc/MyTcp.h \
    ../../../../MyFrame/inc/MyBase64.h \
    ../../../../MyFrame/inc/MyHeap.h \
    ../../../../MyFrame/inc/MyHash.h \
    ../../../../MyFrame/inc/MyMouseEvent.h \
    ../../../../MyFrame/inc/MyAllEvent.h \
    ../../../../MyFrame/inc/MyFrame.h \
    ../../../../MyFrame/inc/MySqlite3.h \
    ../../../../MyFrame/inc/MyKeyEvent.h \
    ../../../../MyFrame/inc/MyNormalEvent.h \
    ../../../../MyFrame/inc/MyTimer.h \
    ../../../../MyFrame/inc/MyControls.h \
    ../../../../MyFrame/inc/MyTest.h \
    ../../../../3rd_src/Sqlite3/sqlite3ext.h \
    ../../../../3rd_src/Sqlite3/sqlite3.h \
    ../inc/MainWidget.h \
    ../inc/MyCapConnect.h \
    ../inc/MyServer.h

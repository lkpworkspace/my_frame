TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# add by myself
INCLUDEPATH += ../../../../MyFrame/inc/ ../inc
LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector

SOURCES += \
    ../../../../MyFrame/src/MyCommon.cpp \
    ../../../../MyFrame/src/MyThread.cpp \
    ../../../../MyFrame/src/MyEvent.cpp \
    ../../../../MyFrame/src/MyApp.cpp \
    ../../../../MyFrame/src/MyLog.cpp \
    ../../../../MyFrame/src/MyHelp.cpp \
    ../../../../MyFrame/src/MyTask.cpp \
    ../../../../MyFrame/src/MyList.cpp \
    ../../../../MyFrame/src/MyControls.cpp \
    ../../../../MyFrame/src/MyNet.cpp \
    ../../../../MyFrame/src/MySock.cpp \
    ../../../../MyFrame/src/MyUdp.cpp \
    ../../../../MyFrame/src/MyTcp.cpp \
    ../../../../MyFrame/src/MyNormalEvent.cpp \
    ../../../../MyFrame/src/MyTimer.cpp \
    ../../../../MyFrame/src/MyAllEvent.cpp \
    ../../../../MyFrame/src/MyWriteable.cpp \
    ../../../../MyFrame/src/MySqlite3.cpp \
    ../../../../3rd_src/Sqlite3/sqlite3.c \
    ../../../../3rd_src/blowfish/blowfish.cpp \
    ../../../../3rd_src/md5/md5.cpp \
    ../src/main.cpp \
    ../src/MyTalkServer.cpp

HEADERS += \
    ../../../../MyFrame/inc/MyCommon.h \
    ../../../../MyFrame/inc/MyHelp.h \
    ../../../../MyFrame/inc/MyLog.h \
    ../../../../MyFrame/inc/MyFrame.h \
    ../../../../MyFrame/inc/MyApp.h \
    ../../../../MyFrame/inc/MyList.h \
    ../../../../MyFrame/inc/MyTask.h \
    ../../../../MyFrame/inc/MyThread.h \
    ../../../../MyFrame/inc/MyEvent.h \
    ../../../../MyFrame/inc/MyTimer.h \
    ../../../../MyFrame/inc/MySock.h \
    ../../../../MyFrame/inc/MyNet.h \
    ../../../../MyFrame/inc/MyNormalEvent.h \
    ../../../../MyFrame/inc/MyControls.h \
    ../../../../MyFrame/inc/MyUdp.h \
    ../../../../MyFrame/inc/MyTcp.h \
    ../../../../MyFrame/inc/MyAllEvent.h \
    ../../../../MyFrame/inc/MySqlite3.h \
    ../../../../3rd_src/Sqlite3/sqlite3.h \
    ../../../../3rd_src/Sqlite3/sqlite3ext.h \
    ../../../../3rd_src/blowfish/blowfish.h \
    ../../../../3rd_src/md5/md5.h \
    ../inc/MyTalkServer.h

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

# add by myself
INCLUDEPATH += ../../../../
LIBS += -lpthread -ldl -lrt
QMAKE_CXXFLAGS += -fno-stack-protector

SOURCES += \
    ../../../../Common.cpp \
    ../../../../MySock.cpp \
    ../../../../MyNet.cpp \
    ../../../../MyThread.cpp \
    ../../../../MyEvent.cpp \
    ../../../../MyApp.cpp \
    ../../../../MyLog.cpp \
    ../../../../MyHelp.cpp \
    ../../../../MyTask.cpp \
    ../../../../MyList.cpp \
    ../../../../MyUdp.cpp \
    ../../../../MyTcp.cpp \
    ../../../../MyBase64.cpp \
    ../../../../MyMouseEvent.cpp \
    ../../../../MyAllEvent.cpp \
    ../../../../3rd_src/Sqlite3/sqlite3.c \
    ../../../../MySqlite3.cpp \
    ../../../../MyTFTP.cpp \
    ../../../../MyKeyEvent.cpp \
    ../../../../MyNormalEvent.cpp \
    main.cpp \
    MyTalkServer.cpp

HEADERS += \
    ../../../../Common.h \
    ../../../../MySock.h \
    ../../../../MyNet.h \
    ../../../../MyThread.h \
    ../../../../MyEvent.h \
    ../../../../MyApp.h \
    ../../../../MyList.h \
    ../../../../MyVec.h \
    ../../../../MyLog.h \
    ../../../../MyHelp.h \
    ../../../../MyTask.h \
    ../../../../MyUdp.h \
    ../../../../MyTcp.h \
    ../../../../MyBase64.h \
    ../../../../MyHeap.h \
    ../../../../MyHash.h \
    ../../../../MyMouseEvent.h \
    ../../../../MyAllEvent.h \
    ../../../../MyFrame.h \
    ../../../../3rd_src/Sqlite3/sqlite3.h \
    ../../../../MySqlite3.h \
    ../../../../MyTFTP.h \
    ../../../../MyKeyEvent.h \
    ../../../../3rd_src/Sqlite3/sqlite3ext.h \
    ../../../../MyNormalEvent.h \
    MyTalkServer.h

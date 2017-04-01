TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lpthread

SOURCES += main.cpp \
    Common.cpp \
    MySock.cpp \
    MyNet.cpp \
    MyThread.cpp \
    MyEvent.cpp \
    MyApp.cpp \
    MyLog.cpp \
    MyHelp.cpp \
    MyTask.cpp \
    MyList.cpp \
    MyUdp.cpp \
    MyTcp.cpp \
    MyBase64.cpp

HEADERS += \
    Common.h \
    MySock.h \
    MyNet.h \
    MyThread.h \
    MyEvent.h \
    MyApp.h \
    MyList.h \
    MyVec.h \
    MyLog.h \
    MyHelp.h \
    MyTask.h \
    MyUdp.h \
    MyTcp.h \
    MyBase64.h \
    MyHeap.h \
    MyHash.h

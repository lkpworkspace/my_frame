TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

unix {
    LIBS += -lpthread -ldl -lrt

    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}

DEFINES += GSERVER
#DEFINES += GSERVER_TEST

# 以下为 Game/server 中的源文件
SOURCES += \
    GClientProxy.cpp \
    GReply.cpp \
    GScene.cpp \
    GServer.cpp \
    GLoginManager.cpp \
    GMatchingManager.cpp \
    GRPCManager.cpp \
    GSceneManager.cpp


HEADERS += \
    GClientProxy.h \
    GReply.h \
    GScene.h \
    GServer.h \
    IGClientProxy.h \
    GLoginManager.h \
    GMatchingManager.h \
    GRPCManager.h \
    GSceneManager.h \
    GTestFunc.h


# 以下为 Game/common 中的源文件
INCLUDEPATH += ../common
SOURCES += \
    ../common/GMsg.cpp \
    ../common/GMsgType.cpp \
    ../common/GUtils.cpp \
    ../common/GNetManager.cpp

HEADERS += \
    ../common/GCommon.h \
    ../common/GMsg.h \
    ../common/GMsgType.h \
    ../common/GUtils.h \
    ../common/IGMsgProcess.h \
    ../common/GNetManager.h


# 以下为 MyFrame 中包含的源文件
INCLUDEPATH += ../../../MyFrame/inc/core
SOURCES += \
    ../../../MyFrame/src/core/MyCommon.cpp \
    ../../../MyFrame/src/core/MyObj.cpp \
    ../../../MyFrame/src/core/MyThread.cpp \
    ../../../MyFrame/src/core/MyEvent.cpp \
    ../../../MyFrame/src/core/MyMsgPool.cpp \
    ../../../MyFrame/src/core/MyApp.cpp \
    ../../../MyFrame/src/core/MyLog.cpp \
    ../../../MyFrame/src/core/MyHelp.cpp \
    ../../../MyFrame/src/core/MyTask.cpp \
    ../../../MyFrame/src/core/MyList.cpp \
    ../../../MyFrame/src/core/MyNet.cpp \
    ../../../MyFrame/src/core/MySock.cpp \
    ../../../MyFrame/src/core/MyUdp.cpp \
    ../../../MyFrame/src/core/MyTcp.cpp \
    ../../../MyFrame/src/core/MyNormalEvent.cpp \
    ../../../MyFrame/src/core/MyTimer.cpp \
    ../../../MyFrame/src/core/MyIOStream.cpp \
    ../../../MyFrame/src/core/MyDataParser.cpp \
    ../../../MyFrame/src/core/MyMath.cpp

HEADERS += \
    ../../../MyFrame/inc/core/MyCommon.h \
    ../../../MyFrame/inc/core/MyObj.h \
    ../../../MyFrame/inc/core/MyHelp.h \
    ../../../MyFrame/inc/core/MyLog.h \
    ../../../MyFrame/inc/core/MyFrame.h \
    ../../../MyFrame/inc/core/MyApp.h \
    ../../../MyFrame/inc/core/MyVec.h \
    ../../../MyFrame/inc/core/MyList.h \
    ../../../MyFrame/inc/core/MyTask.h \
    ../../../MyFrame/inc/core/MyThread.h \
    ../../../MyFrame/inc/core/MyEvent.h \
    ../../../MyFrame/inc/core/MyMsgPool.h \
    ../../../MyFrame/inc/core/MyTimer.h \
    ../../../MyFrame/inc/core/MySock.h \
    ../../../MyFrame/inc/core/MyNet.h \
    ../../../MyFrame/inc/core/MyUdp.h \
    ../../../MyFrame/inc/core/MyTcp.h \
    ../../../MyFrame/inc/core/MyNormalEvent.h \
    ../../../MyFrame/inc/core/MyIOStream.h \
    ../../../MyFrame/inc/core/MyDataParser.h \
    ../../../MyFrame/inc/core/MyMath.h


# main
SOURCES += main.cpp


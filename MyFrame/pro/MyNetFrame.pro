TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../inc ../example ../../3rd_src/ ../inc/MyGameEngine

SOURCES += \
    ../src/MyCommon.cpp \
    ../src/MyObj.cpp \
    ../src/MyThread.cpp \
    ../src/MyEvent.cpp \
    ../src/MyMsgPool.cpp \
    ../src/MyApp.cpp \
    ../src/MyLog.cpp \
    ../src/MyHelp.cpp \
    ../src/MyTask.cpp \
    ../src/MyList.cpp \
    ../src/MyNet.cpp \
    ../src/MySock.cpp \
    ../src/MyUdp.cpp \
    ../src/MyTcp.cpp \
    ../src/MyNormalEvent.cpp \
    ../src/MyTimer.cpp \
    ../src/MySqlite3.cpp \
    ../src/MyIOStream.cpp \
    ../src/MyGameEngine/MyGameObj.cpp \
    ../../3rd_src/Sqlite3/sqlite3.c \
    ../../3rd_src/blowfish/blowfish.cpp \
    ../../3rd_src/md5/md5.cpp \
    ../src/main.cpp

#   ../src/MyWriteable.cpp \
#   ../src/MyControls.cpp \
#   ../src/MyAllEvent.cpp \
#   ../src/MyBase64.cpp \
#   ../src/MyMouseEvent.cpp \
#   ../src/MyKeyEvent.cpp \
#   ../src/MyTFTP.cpp \
#   ../src/MyFileEvent.cpp \
#   ../src/MyTest.cpp \
#   ../src/MyICMP.cpp \
#   ../src/MyRaw.cpp \

HEADERS += \
    ../inc/MyCommon.h \
    ../inc/MyObj.h \
    ../inc/MyHelp.h \
    ../inc/MyLog.h \
    ../inc/MyFrame.h \
    ../inc/MyApp.h \
    ../inc/MyList.h \
    ../inc/MyTask.h \
    ../inc/MyThread.h \
    ../inc/MyEvent.h \
    ../inc/MyMsgPool.h \
    ../inc/MyTimer.h \
    ../inc/MySock.h \
    ../inc/MyNet.h \
    ../inc/MyUdp.h \
    ../inc/MyTcp.h \
    ../inc/MyNormalEvent.h \
    ../inc/MySqlite3.h \
    ../inc/MyIOStream.h \
    ../inc/MyGameEngine/MyGameObj.h \
    ../../3rd_src/Sqlite3/sqlite3.h \
    ../../3rd_src/Sqlite3/sqlite3ext.h \
    ../../3rd_src/blowfish/blowfish.h \
    ../../3rd_src/md5/md5.h \
    ../../3rd_src/nuklear/nuklear.h \
    ../example/MyObj_test.h \
    ../example/MyApp_test.h \
    ../example/MyEvent_test.h \
    ../example/MyHeap_test.h \
    ../example/MyList_test.h \
    ../example/MyNormalEvent_test.h \
    ../example/MySqlite3_test.h \
    ../example/MyTcp_test.h \
    ../example/MyTFTP_test.h \
    ../example/MyTimer_test.h \
    ../example/MyUdp_test.h \
    ../example/MyWriteable_test.h \
    ../example/MyTest.h \
    ../example/MyAllEvent_test.h \
    ../example/MyFrame_test.h \
    ../example/MyNuklear_test.h


#   ../inc/MyWriteable.h \
#   ../inc/MyControls.h \
#   ../inc/MyAllEvent.h \
#   ../inc/MyVec.h \
#   ../inc/MyBase64.h \
#   ../inc/MyHeap.h \
#   ../inc/MyHash.h \
#   ../inc/MyWriteable.h \
#   ../inc/MyMouseEvent.h \
#   ../inc/MyKeyEvent.h \
#   ../inc/MyTFTP.h \
#   ../inc/MyFileEvent.h \
#   ../inc/MyTest.h \
#   ../inc/MyICMP.h \
#   ../inc/MyRaw.h \

unix {
    DISTFILES +=
    LIBS += -lpthread -ldl -lrt
    #LIBS += -lSDL2 -lGL -lm -lGLU -lGLEW
    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}


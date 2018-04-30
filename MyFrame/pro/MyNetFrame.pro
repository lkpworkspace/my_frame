TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../inc/core ../example ../../3rd_src/

SOURCES += \
    ../src/core/MyCommon.cpp \
    ../src/core/MyObj.cpp \
    ../src/core/MyThread.cpp \
    ../src/core/MyEvent.cpp \
    ../src/core/MyMsgPool.cpp \
    ../src/core/MyApp.cpp \
    ../src/core/MyLog.cpp \
    ../src/core/MyHelp.cpp \
    ../src/core/MyTask.cpp \
    ../src/core/MyList.cpp \
    ../src/core/MyNet.cpp \
    ../src/core/MySock.cpp \
    ../src/core/MyUdp.cpp \
    ../src/core/MyTcp.cpp \
    ../src/core/MyNormalEvent.cpp \
    ../src/core/MyTimer.cpp \
    ../src/core/MyIOStream.cpp \
    ../src/core/MyDataParser.cpp \
    ../src/core/MyMath.cpp \
    ../src/core/main.cpp

HEADERS += \
    ../inc/core/MyCommon.h \
    ../inc/core/MyObj.h \
    ../inc/core/MyHelp.h \
    ../inc/core/MyLog.h \
    ../inc/core/MyFrame.h \
    ../inc/core/MyApp.h \
    ../inc/core/MyVec.h \
    ../inc/core/MyList.h \
    ../inc/core/MyTask.h \
    ../inc/core/MyThread.h \
    ../inc/core/MyEvent.h \
    ../inc/core/MyMsgPool.h \
    ../inc/core/MyTimer.h \
    ../inc/core/MySock.h \
    ../inc/core/MyNet.h \
    ../inc/core/MyUdp.h \
    ../inc/core/MyTcp.h \
    ../inc/core/MyNormalEvent.h \
    ../inc/core/MyIOStream.h \
    ../inc/core/MyDataParser.h \
    ../inc/core/MyMath.h \
    ../example/MyObj_test.h \
    ../example/MyApp_test.h \
    ../example/MyHeap_test.h \
    ../example/MyNormalEvent_test.h \
    ../example/MyTFTP_test.h \
    ../example/MyTimer_test.h \
    ../example/MyUdp_test.h \
    ../example/MyWriteable_test.h \
    ../example/MyTest.h \
    ../example/MyAllEvent_test.h \
    ../example/MyFrame_test.h \
    ../example/MyIOStream_test.h

unix {

    DISTFILES +=
    LIBS += -lpthread -ldl -lrt

    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}


#contains(DEFINES,3rd_src){

#    HEADERS += \
#        ../../3rd_src/blowfish/blowfish.h \
#        ../../3rd_src/md5/md5.h

#    SOURCES += \
#        ../../3rd_src/blowfish/blowfish.cpp \
#        ../../3rd_src/md5/md5.cpp

#}

#contains(DEFINES,nuklear){

#    HEADERS += \
#        ../example/MyNuklear_test.h \
#        ../../3rd_src/nuklear/nuklear.h

#    LIBS += -lSDL2 -lGL -lm -lGLU -lGLEW
#}

#contains(DEFINES,mysqlite3){

#    HEADERS += \
#        ../../3rd_src/Sqlite3/sqlite3.h \
#        ../../3rd_src/Sqlite3/sqlite3ext.h \
#        ../inc/MySqlite3.h \
#        ../example/MySqlite3_test.h \

#    SOURCES += \
#        ../../3rd_src/Sqlite3/sqlite3.c \
#        ../src/MySqlite3.cpp
#}


#contains(DEFINES,myextsrc){
#    HEADERS += \
#        ../inc/MyWriteable.h \
#        ../inc/MyControls.h \
#        ../inc/MyAllEvent.h \
#        ../inc/MyBase64.h \
#        ../inc/MyHeap.h \
#        ../inc/MyHash.h \
#        ../inc/MyWriteable.h \
#        ../inc/MyMouseEvent.h \
#        ../inc/MyKeyEvent.h \
#        ../inc/MyTFTP.h \
#        ../inc/MyFileEvent.h \
#        ../inc/MyTest.h \
#        ../inc/MyICMP.h \
#        ../inc/MyRaw.h
#    SOURCES += \
#        ../src/MyWriteable.cpp \
#        ../src/MyControls.cpp \
#        ../src/MyAllEvent.cpp \
#        ../src/MyBase64.cpp \
#        ../src/MyMouseEvent.cpp \
#        ../src/MyKeyEvent.cpp \
#        ../src/MyTFTP.cpp \
#        ../src/MyFileEvent.cpp \
#        ../src/MyTest.cpp \
#        ../src/MyICMP.cpp \
#        ../src/MyRaw.cpp
#}



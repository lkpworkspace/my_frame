TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += mygameengine

INCLUDEPATH += ../inc ../example ../../3rd_src/

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
    ../src/MyIOStream.cpp

HEADERS += \
    ../inc/MyCommon.h \
    ../inc/MyObj.h \
    ../inc/MyHelp.h \
    ../inc/MyLog.h \
    ../inc/MyFrame.h \
    ../inc/MyApp.h \
    ../inc/MyVec.h \
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
    ../inc/MyIOStream.h

unix {

    DISTFILES +=
    LIBS += -lpthread -ldl -lrt

    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}

contains(DEFINES,mygameengine){
    message('[DEFINES]: use_mygameengine')
    INCLUDEPATH += ../inc/MyGameEngine
    # engine <-- server
    HEADERS += \
        ../inc/MyGameEngine/MyGameMsg.h \
        ../inc/MyGameEngine/MyGameObj.h \
        ../inc/MyGameEngine/MyWorld.h \
        ../inc/MyGameEngine/MyEngine.h \
        ../inc/MyGameEngine/MyInputState.h \
        ../inc/MyGameEngine/MyMove.h \
        ../inc/MyGameEngine/MyMoveList.h \
        ../inc/MyGameEngine/MyTiming.h \
        ../inc/MyGameEngine/MyGameEngineShared.h \
        ../inc/MyGameEngine/MyNetworkManager.h \
        ../inc/MyGameEngine/Server/MyClientProxy.h \
        ../inc/MyGameEngine/Server/MyNetworkManagerServer.h \
        ../inc/MyGameEngine/Server/MyServer.h \
        ../inc/MyGameEngine/Server/MyServerShared.h

    SOURCES += \
        ../src/MyGameEngine/MyGameMsg.cpp \
        ../src/MyGameEngine/MyGameObj.cpp \
        ../src/MyGameEngine/MyWorld.cpp \
        ../src/MyGameEngine/MyEngine.cpp \
        ../src/MyGameEngine/MyInputState.cpp \
        ../src/MyGameEngine/MyMove.cpp \
        ../src/MyGameEngine/MyMoveList.cpp \
        ../src/MyGameEngine/MyTiming.cpp \
        ../src/MyGameEngine/MyNetworkManager.cpp \
        ../src/MyGameEngine/Server/MyClientProxy.cpp \
        ../src/MyGameEngine/Server/MyNetworkManagerServer.cpp \
        ../src/MyGameEngine/Server/MyServer.cpp \
        ../src/MyGameEngine/Server/main.cpp

}else{
    message('[DEFINES]: no_mygameengine)')
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



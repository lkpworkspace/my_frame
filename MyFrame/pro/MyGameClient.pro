TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += mygameengine

INCLUDEPATH += ../inc

SOURCES += \
    ../src/MyObj.cpp \
    ../src/MyList.cpp \
    ../src/MyEvent.cpp \
    ../src/MyNet.cpp \
    ../src/MyIOStream.cpp \
    ../src/MyDataParser.cpp \
    ../src/MyMath.cpp

HEADERS += \
    ../inc/MyObj.h \
    ../inc/MyList.h \
    ../inc/MyEvent.h \
    ../inc/MyVec.h \
    ../inc/MyMath.h \
    ../inc/MyIOStream.h \
	../inc/MyDataParser.h

unix {
    DISTFILES +=
    LIBS += -lpthread -ldl -lrt

    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}

contains(DEFINES,mygameengine){
    message('[DEFINES]: use_mygameengine')
    INCLUDEPATH += ../inc/MyGameEngine
    INCLUDEPATH += ../inc/MySimpleClient

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
        ../inc/MyGameEngine/MyPlane.h \
        ../inc/MyGameEngine/Client/MyGameClientShared.h \
        ../inc/MyGameEngine/Client/MyNetworkManagerClient.h \
        ../inc/MyGameEngine/Client/MyPlaneClient.h \
		../inc/MySimpleClient/MySimpleTcpClient.h

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
        ../src/MyGameEngine/MyPlane.cpp \
        ../src/MyGameEngine/Client/MyPlaneClient.cpp \
        ../src/MyGameEngine/Client/MyNetworkManagerClient.cpp \
        ../src/MyGameEngine/Client/main.cpp \
        ../src/MySimpleClient/MySimpleTcpClient.cpp

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



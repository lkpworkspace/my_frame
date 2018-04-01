TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../inc ../inc/MySimpleClient

SOURCES += \
    ../src/MySimpleClient/main.cpp \
    ../src/MySimpleClient/MySimpleTcpClient.cpp \
    ../src/MyNet.cpp \
    ../src/MyDataParser.cpp \
    ../src/MyIOStream.cpp

HEADERS += \
    ../inc/MySimpleClient/MySimpleTcpClient.h \
    ../inc/MyNet.h \
    ../inc/MyVec.h \
    ../inc/MyDataParser.h \
    ../inc/MyIOStream.h

unix {
    #LIBS += -lrt
    #DISTFILES +=
    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}









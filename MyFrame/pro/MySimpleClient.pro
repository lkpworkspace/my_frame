TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../inc ../inc/MySimpleClient

SOURCES += \
    ../src/MySimpleClient/main.cpp \
    ../src/MySimpleClient/MyEasyTcpClient.cpp \
    ../src/MyNet.cpp

HEADERS += \
    ../inc/MySimpleClient/MyEasyTcpClient.h \
    ../inc/MyNet.h \
    ../inc/MyVec.h

unix {
    #LIBS += -lrt
    #DISTFILES +=
    #QMAKE_CXXFLAGS += -fno-stack-protector -Wno-reorder
    #DESTDIR =
}









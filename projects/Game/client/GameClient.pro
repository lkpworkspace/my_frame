#-------------------------------------------------
#
# Project created by QtCreator 2018-05-26T00:34:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GameClient
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# 以下为 MyFrame 中包含的源文件
INCLUDEPATH += ../../../MyFrame/inc/core
SOURCES += \
    ../../../MyFrame/src/core/MyObj.cpp \
    ../../../MyFrame/src/core/MyList.cpp \
    ../../../MyFrame/src/core/MyEvent.cpp \
    ../../../MyFrame/src/core/MyNet.cpp \
    ../../../MyFrame/src/core/MyIOStream.cpp \
    ../../../MyFrame/src/core/MyDataParser.cpp \
    ../../../MyFrame/src/core/MyMath.cpp

HEADERS += \
    ../../../MyFrame/inc/core/MyObj.h \
    ../../../MyFrame/inc/core/MyList.h \
    ../../../MyFrame/inc/core/MyEvent.h \
    ../../../MyFrame/inc/core/MyVec.h \
    ../../../MyFrame/inc/core/MyNet.h \
    ../../../MyFrame/inc/core/MyMath.h \
    ../../../MyFrame/inc/core/MyIOStream.h \
    ../../../MyFrame/inc/core/MyDataParser.h


# 以下为 Game/common 中包含的源文件
# 。。。

# 以下为 Game/client 中包含的源文件
# 。。。




SOURCES += main.cpp\
        MainWindow.cpp

HEADERS  += MainWindow.h

FORMS    += MainWindow.ui

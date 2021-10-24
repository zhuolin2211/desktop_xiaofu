#-------------------------------------------------
#
# Project created by QtCreator 2021-10-20T21:07:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = hardware_test
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    playaudio.cpp \
    key.cpp

HEADERS  += mainwindow.h \
    rtrudio/RtAudio.h \
    playaudio.h \
    key.h

FORMS    += mainwindow.ui

DISTFILES += \
    rtrudio/librtaudio.so.6.0.1
#Compile as arm
LIBS += -L/home/sun/project/desktop_xiaofu/desktop_xiaofu/rtaudio/linux-arm-lib -lrtaudio
#Compile as x86
#LIBS += -L/home/sun/project/rtaudio/rtaudio-master/.libs -lrtaudio

RESOURCES += \
    file.qrc

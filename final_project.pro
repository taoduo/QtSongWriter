#-------------------------------------------------
#
# Project created by QtCreator 2016-03-03T19:41:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = final_project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    bluesUtils.cpp \
    CBluesDrumTrack.cpp \
    CMidiPacket43.cpp \
    CMidiTrack.cpp \
    functorTests.cpp \
    RtMidi.cpp

HEADERS  += mainwindow.h \
    bluesUtils.h \
    CBluesDrumTrack.h \
    CBluesTrack.h \
    CMidiPacket43.h \
    CMidiTrack.h \
    functorTests.h \
    RtMidi.h


DISTFILES += \
    final_project.pro.user \
    drums.txt \
    readme.md
CONFIG += c++11
macx {
    INCLUDEPATH += /Library/Frameworks/
    QMAKE_LFLAGS += -F/Library/Frameworks
    QMAKE_CXXFLAGS += -D__MACOSX_CORE__
    LIBS += -framework CoreFoundation \
        -framework CoreMIDI \
        -framework CoreAudio
    QMAKE_CXXFLAGS_WARN_ON += -Wno-unknown-pragmas
    QMAKE_MAC_SDK = macosx10.10
}

FORMS    += mainwindow.ui

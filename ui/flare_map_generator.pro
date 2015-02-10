#-------------------------------------------------
#
# Project created by QtCreator 2015-01-17T18:56:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = flare_MAP_GENERATOR
TEMPLATE = app

INCLUDEPATH += ../src \
               ../src/flare \

SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/MapHelper.cpp \
    ../src/flare/EventManager.cpp \
    ../src/flare/FileParser.cpp \
    ../src/flare/Map.cpp \
    ../src/flare/Settings.cpp \
    ../src/flare/Utils.cpp \
    ../src/flare/UtilsFileSystem.cpp \
    ../src/flare/UtilsParsing.cpp \
    ../src/MapGenerator_C1.cpp \
    ../src/MapGenerator_D1.cpp \
    ../src/MapGenerator_D2.cpp \
    ../src/MapGenerator_M1.cpp \
    ../src/MapGenerator.cpp \
    ../src/TilesetDef.cpp \
    ../src/flare/MapSaver.cpp

HEADERS  += mainwindow.h \
    ../src/MapHelper.h \
    ../src/MapGenerator_C1.h \
    ../src/MapGenerator_D1.h \
    ../src/MapGenerator_D2.h \
    ../src/MapGenerator_M1.h \
    ../src/MapGenerator.h \
    ../src/TilesetDef.h \
    ../src/flare/CommonIncludes.h \
    ../src/flare/EventManager.h \
    ../src/flare/FileParser.h \
    ../src/flare/Map.h \
    ../src/flare/MapSaver.h \
    ../src/flare/Settings.h \
    ../src/flare/Utils.h \
    ../src/flare/UtilsFileSystem.h \
    ../src/flare/UtilsMath.h \
    ../src/flare/UtilsParsing.h

INCLUDEPATH += flare \
               export

FORMS    += mainwindow.ui

CONFIG   += debug_and_release


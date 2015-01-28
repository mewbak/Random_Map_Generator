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
               ../src/export \

SOURCES += main.cpp\
        mainwindow.cpp \
    ../src/a-star.cpp \
    ../src/MapHelper.cpp \
    ../src/map_gen_BSP.cpp \
    ../src/map_gen_M1.cpp \
    ../src/map_gen_RC.cpp \
    ../src/map_gen_RR.cpp \
    ../src/flare/EventManager.cpp \
    ../src/flare/FileParser.cpp \
    ../src/flare/Map.cpp \
    ../src/flare/Settings.cpp \
    ../src/flare/Utils.cpp \
    ../src/flare/UtilsFileSystem.cpp \
    ../src/flare/UtilsParsing.cpp \
    ../src/export/MapSaver.cpp \
    ../src/MapGenerator_C1.cpp \
    ../src/MapGenerator_D1.cpp \
    ../src/MapGenerator.cpp

HEADERS  += mainwindow.h \
    ../src/a-star.h \
    ../src/MapHelper.h \
    ../src/map_gen_BSP.h \
    ../src/map_gen_M1.h \
    ../src/map_gen_RC.h \
    ../src/map_gen_RR.h \
    ../src/MapGenerator_C1.h \
    ../src/MapGenerator_D1.h \
    ../src/MapGenerator.h

INCLUDEPATH += flare \
               export

FORMS    += mainwindow.ui


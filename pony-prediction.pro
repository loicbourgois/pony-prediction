#-------------------------------------------------
#
# Project created by QtCreator 2015-04-01T20:55:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pony-prediction
TEMPLATE = app


SOURCES += \
    main.cpp \
    ui/main-window.cpp \
    core/simulation.cpp

HEADERS  += \
    ui/main-window.hpp \
    core/simulation.hpp

FORMS    += \
    ui/main-window.ui

CONFIG += mobility
MOBILITY = 


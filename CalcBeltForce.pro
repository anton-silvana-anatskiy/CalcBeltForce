#-------------------------------------------------
#
# Project created by QtCreator 2016-04-05T20:34:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CalcBeltForce
TEMPLATE = app


SOURCES += main.cpp\
        dialog.cpp \
    section.cpp \
    forcefactor.cpp

HEADERS  += dialog.hpp \
    section.hpp \
    forcefactor.hpp

FORMS    += dialog.ui \
    forcefactor.ui

QMAKE_CXXFLAGS += -std=c++11


TEMPLATE = app
TARGET = test_location

QT       += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../../

win32:INCLUDEPATH += ../../db-wrapper/soci/core \
                     ../../db-wrapper/soci/backend \
                     ../../db-wrapper/ibase \

unix:INCLUDEPATH += /usr/local/include/soci \
                    /usr/local/include/soci/firebird/ \

win32:LIBS += -L$$quote(../../../db-wrapper/soci/lib) \
              -l$$quote(soci_core_3_2) \
              -l$$quote(soci_firebird_3_2) \
              -l$$quote(soci_empty_3_2)

unix:LIBS += -lsoci_core \
             -lsoci_firebird

HEADERS += testlocation.h
SOURCES += testlocation.cpp main.cpp

include(sors.pri)

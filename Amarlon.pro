#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T23:45:22
#
#-------------------------------------------------

QT       += core gui
QT  += testlib
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Amarlon
TEMPLATE = app

INCLUDEPATH += /usr/local/include/soci \
               /usr/local/include/soci/firebird/ \
               ../../include

LIBS += -lsoci_core \
        -lsoci_firebird

SOURCES += main.cpp\
        game.cpp \
    Tests/testlocation.cpp \
    World/location.cpp \
    Tests/testdb.cpp \
    Include/func.cpp \
    Include/enums.cpp \
    Equipment/item.cpp \
    Include/db.cpp \
    Tests/testitems.cpp \
    Equipment/ordinaryitem.cpp \
    Equipment/weapon.cpp \
    Equipment/armor.cpp \
    Equipment/jewelry.cpp \
    Equipment/food.cpp \
    Equipment/tool.cpp \
    Include/comobj.cpp \
    Equipment/shield.cpp \
    World/locationobject.cpp

HEADERS  += game.h \
    Tests/testlocation.h \
    World/location.h \
    Include/enums.h \
    Tests/testdb.h \
    Include/func.h \
    Include/inc.h \
    Include/exceptions.h \
    Equipment/item.h \
    Include/prototypemanager.h \
    Include/db.h \
    Tests/testitems.h \
    Equipment/ordinaryitem.h \
    Equipment/weapon.h \
    Equipment/armor.h \
    Equipment/jewelry.h \
    Equipment/food.h \
    Equipment/tool.h \
    Include/comobj.h \
    Equipment/shield.h \
    World/locationobject.h

FORMS    += game.ui

OTHER_FILES += \
    TODO.txt

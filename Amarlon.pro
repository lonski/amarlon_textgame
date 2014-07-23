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

INCLUDEPATH += ../../include

win32:INCLUDEPATH += $$PWD/db-wrapper/soci/core \
                     $$PWD/db-wrapper/soci/backend \
                     $$PWD/db-wrapper/ibase \

unix:INCLUDEPATH += /usr/local/include/soci \
                    /usr/local/include/soci/firebird/ \

win32:LIBS += -L$$quote($$PWD/db-wrapper/soci/lib) \
              -l$$quote(soci_core_3_2) \
              -l$$quote(soci_firebird_3_2) \
              -l$$quote(soci_empty_3_2)

unix:LIBS += -lsoci_core \
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
    World/locationobject.cpp \
    Creatures/creature.cpp \
    Tests/testfun.cpp \
    Tests/testcreature.cpp \
    Creatures/creaturemodificator.cpp \
    Creatures/creaturestats.cpp \
    Creatures/bodypart.cpp \
    Include/gameclock.cpp \
    Creatures/mob.cpp \
    Creatures/npc.cpp \
    Creatures/creaturemonitor.cpp \
    Creatures/player.cpp \
    Commands/command.cpp \
    Commands/commandlogger.cpp \
    Tests/testcommands.cpp \
    Commands/commandparser.cpp \
    Commands/commandexecutor.cpp \
    Commands/go.cpp \
    Commands/take.cpp \
    console.cpp \
    Commands/exit.cpp \
    Commands/gamemenu.cpp \
    Commands/debug.cpp \
    Include/inifile.cpp

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
    World/locationobject.h \
    Creatures/creature.h \
    Tests/testfun.h \
    Tests/testcreature.h \
    Creatures/creaturemodificator.h \
    Creatures/creaturestats.h \
    Creatures/bodypart.h \
    Include/gameclock.h \
    Creatures/mob.h \
    Creatures/npc.h \
    Creatures/creaturemonitor.h \
    Creatures/player.h \
    Commands/command.h \
    Commands/commandlogger.h \
    Tests/testcommands.h \
    Commands/commandparser.h \
    Commands/commandexecutor.h \
    Commands/go.h \
    Commands/take.h \
    console.h \
    Commands/exit.h \
    Commands/gamemenu.h \
    Commands/debug.h \
    Include/inifile.h

FORMS    += game.ui \
    console.ui

OTHER_FILES += \
    TODO.txt

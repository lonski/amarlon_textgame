#-------------------------------------------------
#
# Project created by QtCreator 2014-05-26T23:45:22
#
#-------------------------------------------------

QT       += core gui
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
       Gui/game.cpp \
   World/location.cpp \
   Equipment/item.cpp \
   Include/db.cpp \
   World/locationobject.cpp \
   Creatures/creature.cpp \
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
   Commands/commandparser.cpp \
   Commands/commandexecutor.cpp \
   Commands/go.cpp \
   Commands/take.cpp \
   Commands/exit.cpp \
   Commands/gamemenu.cpp \
   Commands/debug.cpp \
   Include/inifile.cpp \
   Console/consolefonts.cpp \
    World/walkvector.cpp \
    World/locationmanager.cpp \
    Equipment/item_container.cpp \
    Creatures/creaturecontainer.cpp \
    Creatures/body.cpp \
    Creatures/creaturemodificatormanager.cpp \
    Creatures/creaturemanager.cpp \
    Include/functions/enum2str.cpp \
    Include/functions/string_utils.cpp \
    Include/functions/db_utils.cpp \
    Include/functions/common_utils.cpp \
    Gui/gui_console.cpp \
    Include/data_gateways/db_gateways/items_gateway_db.cpp \
    Include/data_gateways/itemsgateway.cpp

HEADERS  += Gui/game.h \
    World/location.h \
    Include/exceptions.h \
    Equipment/item.h \
    Include/prototypemanager.h \
    Include/db.h \
    World/locationobject.h \
    Creatures/creature.h \
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
    Commands/commandparser.h \
    Commands/commandexecutor.h \
    Commands/go.h \
    Commands/take.h \
    Commands/exit.h \
    Commands/gamemenu.h \
    Commands/debug.h \
    Include/inifile.h \
    Console/consolefonts.h \
    World/walkvector.h \
    World/locationmanager.h \
    Equipment/item_container.h \
    Creatures/creaturecontainer.h \
    Creatures/body.h \
    Creatures/creaturemodificatormanager.h \
    Creatures/creaturemanager.h \
    Include/typedefs/def_basetypes.h \
    Include/typedefs/def_db_item.h \
    Include/typedefs/def_db_table.h \
    Include/typedefs/def_measures.h \
    Include/typedefs/def_time.h \
    Include/enums/e_direction.h \
    Include/enums/e_objtype.h \
    Include/enums/e_itemtype.h \
    Include/enums/e_itemprototype.h \
    Include/enums/e_body.h \
    Include/enums/e_damagelevel.h \
    Include/enums/e_itemcondition.h \
    Include/enums/e_weaponskill.h \
    Include/enums/e_attribute.h \
    Include/enums/e_skill.h \
    Include/enums/e_profession.h \
    Include/enums/e_season.h \
    Include/enums/e_sex.h \
    Include/enums/e_creatureprototype.h \
    Include/enums/e_creaturetype.h \
    Include/enums/e_tribe.h \
    Include/enums/e_clan.h \
    Include/enums/e_bless.h \
    Include/enums/e_command.h \
    Include/enums/e_fatiguelevel.h \
    Include/enums/e_hungerlevel.h \
    Include/enums/e_refdict.h \
    Include/observer.h \
    Include/damage.h \
    Include/custom_values.h \
    Include/functions/enum2str.h \
    Include/functions/string_utils.h \
    Include/functions/db_utils.h \
    Include/functions/messages.h \
    Include/functions/common_utils.h \
    Gui/gui_console.h \
    Console/console.h \
    Include/enums/e_font.h \
    Include/data_gateways/data_gateway.h \
    Include/data_gateways/db_gateways/items_gateway_db.h \
    Include/data_gateways/itemsgateway.h

FORMS    += Gui/game.ui \
    Gui/gui_console.ui

OTHER_FILES += \
    TODO.txt

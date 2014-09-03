TEMPLATE = app
TARGET = test_location

QT       += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG += console

DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ../

win32:INCLUDEPATH += ../db-wrapper/soci/core \
                     ../db-wrapper/soci/backend \
                     ../db-wrapper/ibase \

unix:INCLUDEPATH += /usr/local/include/soci \
                    /usr/local/lib64/ \
                    /usr/local/include/soci/firebird/ \

win32:LIBS += -L$$quote($$PWD/../db-wrapper/soci/lib) \
              -l$$quote(soci_core_3_2) \
              -l$$quote(soci_firebird_3_2) \
              -l$$quote(soci_empty_3_2)

unix:LIBS += -lsoci_core \
             -lsoci_firebird

SOURCES += unit_tests_main.cpp \
    testsuite.cpp \
    Test_Creature/testcreaturemodificator.cpp \
    Test_Creature/testcreaturestats.cpp \
    Test_Creature/testbodypart.cpp \
    Test_Creature/testcreaturemodificatormanager.cpp \
    Test_Creature/testcreaturecontainer.cpp \
    Test_Gateways/test_item_containers_gateway.cpp \
    Test_Gateways/test_item_gateway_db.cpp

include(tests.pri)
include(sors.pri)

HEADERS += \
    testsuite.h \
    Test_Creature/testcreaturemodificator.h \
    Test_Creature/testcreaturestats.h \
    Test_Creature/testbodypart.h \
    Test_Creature/testcreaturemodificatormanager.h \
    Test_Creature/testcreaturecontainer.h \
    Test_Gateways/test_item_containers_gateway.h \
    Test_Gateways/test_item_gateway_db.h


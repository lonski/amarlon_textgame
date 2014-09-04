TEMPLATE = app
TARGET = test_location

QT       += core gui testlib
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
QMAKE_LDFLAGS += -fprofile-arcs -ftest-coverage
LIBS += -lgcov

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
    testsuite.cpp


HEADERS += \
    testsuite.h

include(tests.pri)
include(sors.pri)


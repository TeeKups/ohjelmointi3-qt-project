QT += testlib core gui widgets network multimedia

CONFIG += qt console warn_on depend_includepath testcase c++14
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_statisticstest.cpp \
        ../../Game/stats/statistics.cpp




DEFINES += SRCDIR=\\\"$$PWD/\\\"



HEADERS += \
    ../../Game/stats/statistics.hh \
    ../../Course/Courselib/interfaces/istatistics.hh

INCLUDEPATH += \
            ../../Game/ \
            ../../Course/Courselib/ \


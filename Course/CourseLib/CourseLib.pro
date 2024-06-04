#-------------------------------------------------
#
# Project created by Minna at some point in time
#
#-------------------------------------------------

TEMPLATE = lib
CONFIG    += c++14 staticlib
QT        += widgets gui network core multimedia


SOURCES += \
    actors/nysse.cc \
    actors/passenger.cc \
    actors/stop.cc \
    core/location.cc \
    core/logic.cc \
    errors/gameerror.cc \
    errors/initerror.cc \
    graphics/simpleactoritem.cpp \
    graphics/simplemainwindow.cpp \
    offlinereader.cc

HEADERS += \
    actors/nysse.hh \
    actors/passenger.hh \
    actors/stop.hh \
    core/location.hh \
    core/logic.hh \
    creategame.hh \
    doxygeninfo.hh \
    errors/gameerror.hh \
    errors/initerror.hh \
    graphics/simpleactoritem.hh \
    graphics/simplemainwindow.hh \
    interfaces/iactor.hh \
    interfaces/icity.hh \
    interfaces/ipassenger.hh \
    interfaces/istatistics.hh \
    interfaces/istop.hh \
    interfaces/ivehicle.hh \
    offlinereader.hh

FORMS += \
    graphics/simplemainwindow.ui

RESOURCES += \
    offlinedata.qrc

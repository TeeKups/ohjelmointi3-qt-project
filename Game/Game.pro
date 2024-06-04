TEMPLATE = app
TARGET = NYSSE

QT += core gui widgets network multimedia

CONFIG += c++14

SOURCES += \
    gui/betteractoritem.cpp \
    actors/bullet.cpp \
    core/city.cpp \
#    core/creategame.cpp \
    core/engine.cpp \
    gui/gamevieweventfilter.cpp \
    gui/gamewidget.cpp \
    main.cc \
    gui/mainwindow.cpp \
    misc/math.cpp \
    actors/player.cpp \
    stats/statistics.cpp

win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/release/ -lCourseLib
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/debug/ -lCourseLib
else:unix: LIBS += \
    -L$$OUT_PWD/../Course/CourseLib/ -lCourseLib

INCLUDEPATH += \
    $$PWD/../Course/CourseLib

DEPENDPATH += \
    $$PWD/../Course/CourseLib

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/libCourseLib.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/libCourseLib.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/release/CourseLib.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/debug/CourseLib.lib
else:unix: PRE_TARGETDEPS += \
    $$OUT_PWD/../Course/CourseLib/libCourseLib.a

HEADERS += \
    gui/betteractoritem.hh \
    actors/bullet.hh \
    core/city.hh \
    core/engine.hh \
    gui/gamevieweventfilter.hh \
    gui/gamewidget.hh \
    gui/mainwindow.hh \
    misc/math.hh \
    actors/player.hh \
    stats/statistics.hh \
    misc/types.hh

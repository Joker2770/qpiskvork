QT       += core gui multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/EngineLoader.cpp \
    src/FreeStyleGomoku.cpp \
    src/PlayerSettingDialog.cpp \
    src/S2BResDialog.cpp \
    src/StandardGomoku.cpp \
    src/SGFOption.cpp \
    src/customs.cpp \
    src/renju.cpp \
    src/caro.cpp \
    src/board.cpp \
    src/commander.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/manager.cpp \
    src/player.cpp \
    src/timer.cpp

HEADERS += \
    src/EngineLoader.h \
    src/FreeStyleGomoku.h \
    src/PlayerSettingDialog.h \
    src/S2BResDialog.h \
    src/StandardGomoku.h \
    src/SGFOption.h \
    src/customs.h \
    src/renju.h \
    src/caro.h \
    src/board.h \
    src/commander.h \
    src/commands.h \
    src/mainwindow.h \
    src/manager.h \
    src/observer.h \
    src/player.h \
    src/rules.h \
    src/subject.h \
    src/timer.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

TRANSLATIONS += \
    language/zh_CN.ts

win32:CONFIG(release, debug|release): LIBS += -L'C:/Program Files (x86)/libsgfc++/release/' -lliblibsgfcplusplus.dll
else:win32:CONFIG(debug, debug|release): LIBS += -L'C:/Program Files (x86)/libsgfc++/debug/' -lliblibsgfcplusplus.dll
else:unix: LIBS += -L'C:/Program Files (x86)/libsgfc++/' -lliblibsgfcplusplus.dll

INCLUDEPATH += 'C:/Program Files (x86)/libsgfc++/include'
DEPENDPATH += 'C:/Program Files (x86)/libsgfc++/include'

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/libsgfc++/release/liblibsgfcplusplus.dll.a'
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/libsgfc++/debug/liblibsgfcplusplus.dll.a'
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/libsgfc++/release/liblibsgfcplusplus.dll.lib'
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += 'C:/Program Files (x86)/libsgfc++/debug/liblibsgfcplusplus.dll.lib'
else:unix: PRE_TARGETDEPS += 'C:/Program Files (x86)/libsgfc++/liblibsgfcplusplus.dll.a'

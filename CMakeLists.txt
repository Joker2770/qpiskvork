cmake_minimum_required(VERSION 3.5)

project(qpiskvork VERSION 0.5 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Multimedia)

set(PROJECT_SOURCES
	src/FreeStyleGomoku.cpp
	src/StandardGomoku.cpp
	src/renju.cpp
	src/caro.cpp
	src/board.cpp
	src/main.cpp
	src/mainwindow.cpp
	src/manager.cpp
	src/player.cpp
	src/timer.cpp
	src/EngineLoader.cpp
	src/commander.cpp
	src/PlayerSettingDialog.cpp
	src/S2BResDialog.cpp
	src/FreeStyleGomoku.h
	src/StandardGomoku.h
	src/renju.h
	src/caro.h
	src/board.h
	src/commands.h
	src/mainwindow.h
	src/manager.h
	src/observer.h
	src/player.h
	src/rules.h
	src/subject.h
	src/timer.h
	src/EngineLoader.h
	src/commander.h
	src/PlayerSettingDialog.h
	src/S2BResDialog.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qpiskvork
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qpiskvork APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qpiskvork SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qpiskvork
            ${PROJECT_SOURCES}
            res.qrc
        )
    endif()
endif()

target_link_libraries(qpiskvork PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia)

set_target_properties(qpiskvork PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qpiskvork)
endif()

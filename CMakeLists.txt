cmake_minimum_required(VERSION 3.5)

project(qpiskvork VERSION 0.5 LANGUAGES CXX)

# add submodule
# add_subdirectory(libsgfcplusplus)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets Multimedia)
if(WIN32)
	find_package(libsgfcplusplus REQUIRED
			 PATHS "C:/Program Files (x86)/libsgfc++"
			 NO_DEFAULT_PATH)
else()
	find_package(libsgfcplusplus REQUIRED
			PATHS "/usr/local/lib/libsgfcplusplus"
			NO_DEFAULT_PATH)
endif()

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
	src/customs.cpp
	src/SGFOption.cpp
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
	src/customs.h
	src/SGFOption.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${PROJECT_NAME}
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        res.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${PROJECT_NAME} SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${PROJECT_NAME}
            ${PROJECT_SOURCES}
            res.qrc
        )
    endif()
endif()

# add_library(libsgfcplusplus SHARED libsgfcplusplus)
# target_include_directories(${PROJECT_NAME} PRIVATE libsgfcplusplus/include)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Multimedia libsgfcplusplus::libsgfcplusplus_shared)

set_target_properties(${PROJECT_NAME} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

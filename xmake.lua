
add_rules("mode.debug", "mode.release")

target("qpiskvork")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")

    -- qt moc
    add_files("src/EngineLoader.h")
    add_files("src/mainwindow.h")
    add_files("src/PlayerSettingDialog.h")

    add_files("res.qrc")
    add_frameworks("QtCore", "QtWidgets", "QtGui")

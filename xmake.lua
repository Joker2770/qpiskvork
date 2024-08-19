
add_rules("mode.debug", "mode.release")

package("libsgfcplusplus")
    add_deps("cmake")
    set_sourcedir(path.join(os.scriptdir(), "libsgfcplusplus"))
    on_install(function (package)
        local configs = {}
        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("libsgfcplusplus")

target("qpiskvork")
    add_rules("qt.widgetapp")
    add_headerfiles("src/*.h")
    add_files("src/*.cpp")

    -- qt moc
    add_files("src/EngineLoader.h")
    add_files("src/mainwindow.h")
    add_files("src/PlayerSettingDialog.h")
    add_files("src/S2BResDialog.h")
    add_files("res.qrc")

    add_packages("libsgfcplusplus")
    add_ldflags("-llibsgfcplusplus")
    add_frameworks("QtCore", "QtWidgets", "QtGui", "QtMultimedia")
    add_rpathdirs("@loader_path", "@loader_path/lib", "@executable_path", "@executable_path/lib")

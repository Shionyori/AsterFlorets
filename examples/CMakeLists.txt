if(WIN32)
    add_executable(ComponentGallery WIN32 ComponentGallery.cpp)
    add_executable(LayoutDemo WIN32 LayoutDemo.cpp)
    add_executable(ReleaseConsoleDemo WIN32 ReleaseConsoleDemo.cpp)
else()
    add_executable(ComponentGallery ComponentGallery.cpp)
    add_executable(LayoutDemo LayoutDemo.cpp)
    add_executable(ReleaseConsoleDemo ReleaseConsoleDemo.cpp)
endif()

target_link_libraries(ComponentGallery PRIVATE AsterFlorets Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(LayoutDemo PRIVATE AsterFlorets Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(ReleaseConsoleDemo PRIVATE AsterFlorets Qt${QT_VERSION_MAJOR}::Widgets)

if(WIN32)
    get_target_property(_qt_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qt_qmake_executable}" DIRECTORY)
    set(_windeployqt_executable "${_qt_bin_dir}/windeployqt.exe")

    function(aster_deploy_qt_runtime target)
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND "${_windeployqt_executable}" --no-translations --no-compiler-runtime "$<TARGET_FILE:${target}>"
            COMMENT "Deploying Qt runtime for ${target}"
            VERBATIM
        )
    endfunction()

    aster_deploy_qt_runtime(ComponentGallery)
    aster_deploy_qt_runtime(LayoutDemo)
    aster_deploy_qt_runtime(ReleaseConsoleDemo)
endif()

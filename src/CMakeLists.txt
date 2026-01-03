set(ASTERUI_SOURCES
    AsterUI.cpp
)

set(ASTERUI_HEADERS
    ../include/AsterUI/Global.h
    ../include/AsterUI/AsterUI.h
)

add_library(AsterUI SHARED
    ${ASTERUI_SOURCES}
    ${ASTERUI_HEADERS}
)

target_include_directories(AsterUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AsterUI PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_compile_definitions(AsterUI PRIVATE ASTERUI_LIBRARY)

if(WIN32)
    set_target_properties(AsterUI PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

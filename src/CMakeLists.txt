set(ASTERUI_SOURCES
    AsterUI.cpp
    AsterTheme.cpp
    AsterButton.cpp
    AsterAvatar.cpp
    AsterLineEdit.cpp
    AsterCheckBox.cpp
    AsterRadio.cpp
    AsterSwitch.cpp
    AsterCard.cpp
    AsterTag.cpp
    AsterSlider.cpp
    AsterSelect.cpp
    AsterTextEdit.cpp
    AsterDivider.cpp
    AsterProgress.cpp
)

set(ASTERUI_HEADERS
    ../include/AsterUI/Global.h
    ../include/AsterUI/AsterUI.h
    ../include/AsterUI/AsterTheme.h
    ../include/AsterUI/AsterButton.h
    ../include/AsterUI/AsterAvatar.h
    ../include/AsterUI/AsterLineEdit.h
    ../include/AsterUI/AsterTextEdit.h
    ../include/AsterUI/AsterCheckBox.h
    ../include/AsterUI/AsterRadio.h
    ../include/AsterUI/AsterSwitch.h
    ../include/AsterUI/AsterCard.h
    ../include/AsterUI/AsterTag.h
    ../include/AsterUI/AsterSlider.h
    ../include/AsterUI/AsterSelect.h
    ../include/AsterUI/AsterDivider.h
    ../include/AsterUI/AsterProgress.h
)

add_library(AsterUI SHARED
    ${ASTERUI_SOURCES}
    ${ASTERUI_HEADERS}
)

target_include_directories(AsterUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AsterUI PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_compile_definitions(AsterUI PRIVATE ASTERUI_LIBRARY)

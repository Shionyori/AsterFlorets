set(ASTERUI_SOURCES
    AsterUI.cpp
    AsterTheme.cpp
    AsterGrid.cpp
    AsterRow.cpp
    AsterColumn.cpp
    AsterFlow.cpp
    AsterIcon.cpp
    AsterText.cpp
    AsterTitle.cpp
    AsterSpace.cpp
    AsterButton.cpp
    AsterAvatar.cpp
    AsterTextInput.cpp
    AsterCheckBox.cpp
    AsterRadio.cpp
    AsterSwitch.cpp
    AsterCard.cpp
    AsterTag.cpp
    AsterSlider.cpp
    AsterSelect.cpp
    AsterTextArea.cpp
    AsterDivider.cpp
    AsterProgress.cpp
    AsterScrollArea.cpp
    AsterMessage.cpp
)

set(ASTERUI_HEADERS
    ../include/AsterUI/Global.h
    ../include/AsterUI/AsterUI.h
    ../include/AsterUI/AsterTheme.h
    ../include/AsterUI/AsterGrid.h
    ../include/AsterUI/AsterRow.h
    ../include/AsterUI/AsterColumn.h
    ../include/AsterUI/AsterFlow.h
    ../include/AsterUI/AsterIcon.h
    ../include/AsterUI/AsterText.h
    ../include/AsterUI/AsterTitle.h
    ../include/AsterUI/AsterSpace.h
    ../include/AsterUI/AsterTheme.h
    ../include/AsterUI/AsterIcon.h
    ../include/AsterUI/AsterButton.h
    ../include/AsterUI/AsterAvatar.h
    ../include/AsterUI/AsterTextInput.h
    ../include/AsterUI/AsterTextArea.h
    ../include/AsterUI/AsterCheckBox.h
    ../include/AsterUI/AsterRadio.h
    ../include/AsterUI/AsterSwitch.h
    ../include/AsterUI/AsterCard.h
    ../include/AsterUI/AsterTag.h
    ../include/AsterUI/AsterSlider.h
    ../include/AsterUI/AsterSelect.h
    ../include/AsterUI/AsterDivider.h
    ../include/AsterUI/AsterProgress.h
    ../include/AsterUI/AsterScrollArea.h
    ../include/AsterUI/AsterMessage.h
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

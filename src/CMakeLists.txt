set(AsterFlorets_SOURCES
    AsterFlorets.cpp
    AsterTheme.cpp
    components/AsterEqualGrid.cpp
    components/AsterRow.cpp
    components/AsterColumn.cpp
    components/AsterFlow.cpp
    components/AsterIcon.cpp
    components/AsterText.cpp
    components/AsterTitle.cpp
    components/AsterSpace.cpp
    components/AsterButton.cpp
    components/AsterAvatar.cpp
    components/AsterTextInput.cpp
    components/AsterCheckBox.cpp
    components/AsterRadio.cpp
    components/AsterSwitch.cpp
    components/AsterCard.cpp
    components/AsterTag.cpp
    components/AsterSlider.cpp
    components/AsterSelect.cpp
    components/AsterTextArea.cpp
    components/AsterDivider.cpp
    components/AsterLayout.cpp
    components/AsterProgress.cpp
    components/AsterScrollArea.cpp
    components/AsterMessage.cpp
    components/AsterModal.cpp
    components/AsterNotification.cpp
    components/AsterMenu.cpp
    components/AsterBadge.cpp
    components/AsterTabs.cpp
    components/AsterBreadcrumb.cpp
    components/AsterAlert.cpp
    components/AsterDrawer.cpp
    components/AsterSpin.cpp
    components/AsterTooltip.cpp
    components/AsterPopover.cpp
    components/AsterDropdown.cpp
    components/AsterCollapse.cpp
    components/AsterResult.cpp
)

set(AsterFlorets_HEADERS
    ../include/AsterGlobal.h
    ../include/AsterFlorets.h
    ../include/AsterTheme.h
    ../include/AsterEqualGrid.h
    ../include/AsterRow.h
    ../include/AsterColumn.h
    ../include/AsterFlow.h
    ../include/AsterIcon.h
    ../include/AsterText.h
    ../include/AsterTitle.h
    ../include/AsterSpace.h
    ../include/AsterTheme.h
    ../include/AsterIcon.h
    ../include/AsterButton.h
    ../include/AsterAlert.h
    ../include/AsterDrawer.h
    ../include/AsterSpin.h
    ../include/AsterTooltip.h
    ../include/AsterPopover.h
    ../include/AsterDropdown.h
    ../include/AsterCollapse.h
    ../include/AsterResult.h
    ../include/AsterAvatar.h
    ../include/AsterTextInput.h
    ../include/AsterTextArea.h
    ../include/AsterCheckBox.h
    ../include/AsterRadio.h
    ../include/AsterSwitch.h
    ../include/AsterCard.h
    ../include/AsterTag.h
    ../include/AsterSlider.h
    ../include/AsterSelect.h
    ../include/AsterDivider.h
    ../include/AsterLayout.h
    ../include/AsterProgress.h
    ../include/AsterScrollArea.h
    ../include/AsterMessage.h
    ../include/AsterModal.h
    ../include/AsterNotification.h
    ../include/AsterMenu.h
    ../include/AsterBreadcrumb.h
    ../include/AsterTabs.h
    ../include/AsterBadge.h
)

add_library(AsterFlorets SHARED
    ${AsterFlorets_SOURCES}
    ${AsterFlorets_HEADERS}
)

target_include_directories(AsterFlorets PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(AsterFlorets PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)

target_compile_definitions(AsterFlorets PRIVATE AsterFlorets_LIBRARY)

# 安装库文件
install(TARGETS AsterFlorets
    EXPORT AsterFloretsTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# 安装头文件
install(DIRECTORY ../include/ DESTINATION include)


set(AsterFlorets_TEST_SOURCES
    AsterFloretsSmokeTests.cpp
)

add_executable(AsterFloretsSmokeTests
    ${AsterFlorets_TEST_SOURCES}
)

target_link_libraries(AsterFloretsSmokeTests PRIVATE
    AsterFlorets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Test
)

add_test(NAME AsterFloretsSmokeTests COMMAND AsterFloretsSmokeTests)

set_tests_properties(AsterFloretsSmokeTests PROPERTIES
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:AsterFloretsSmokeTests>"
    ENVIRONMENT "QT_QPA_PLATFORM=offscreen;PATH=$<TARGET_FILE_DIR:Qt6::Core>;$ENV{PATH}"
)

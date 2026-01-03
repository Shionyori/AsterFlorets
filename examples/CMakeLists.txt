add_executable(demo main.cpp)

target_link_libraries(demo PRIVATE AsterUI Qt${QT_VERSION_MAJOR}::Widgets)

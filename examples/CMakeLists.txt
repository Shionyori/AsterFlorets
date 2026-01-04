if(WIN32)
    add_executable(demo WIN32 main.cpp)
else()
    add_executable(demo main.cpp)
endif()

target_link_libraries(demo PRIVATE AsterUI Qt${QT_VERSION_MAJOR}::Widgets)

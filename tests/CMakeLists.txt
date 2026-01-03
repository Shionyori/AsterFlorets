find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

add_executable(test_basic tst_basic.cpp)
target_link_libraries(test_basic PRIVATE AsterUI Qt${QT_VERSION_MAJOR}::Test)

add_test(NAME test_basic COMMAND test_basic)

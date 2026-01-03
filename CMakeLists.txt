cmake_minimum_required(VERSION 3.16)

project(AsterUI VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive- /utf-8)
endif()

# 启用自动 MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt 包
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

# 输出配置信息
message(STATUS "AsterUI Configuration:")
message(STATUS "  Qt Version: ${QT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")

# 添加库目录
add_subdirectory(src)

# 添加示例目录 (可选)
option(ASTERUI_BUILD_EXAMPLES "Build examples" ON)
if(ASTERUI_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 添加测试目录 (可选)
option(ASTERUI_BUILD_TESTS "Build tests" OFF)
if(ASTERUI_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

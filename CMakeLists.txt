cmake_minimum_required(VERSION 3.16)

project(AsterFlorets VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # DLL / .so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)  # .lib / .a

if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive- /utf-8)
endif()

# 启用自动 MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找 Qt 包
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 输出配置信息
message(STATUS "AsterFlorets Configuration:")
message(STATUS "  Qt Version: ${QT_VERSION}")
message(STATUS "  C++ Standard: ${CMAKE_CXX_STANDARD}")

# 添加库目录
add_subdirectory(src)

# 添加演示目录 (可选)
option(ASTER_BUILD_EXAMPLES "Build examples" ON)
if(ASTER_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# 生成并安装 CMake 配置文件
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/AsterFloretsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/AsterFloretsConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/AsterFloretsConfig.cmake"
    INSTALL_DESTINATION lib/cmake/AsterFlorets
)

install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/AsterFloretsConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/AsterFloretsConfigVersion.cmake"
    DESTINATION lib/cmake/AsterFlorets
)

install(
    EXPORT AsterFloretsTargets
    FILE AsterFloretsTargets.cmake
    NAMESPACE AsterFlorets::
    DESTINATION lib/cmake/AsterFlorets
)


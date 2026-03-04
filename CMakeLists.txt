cmake_minimum_required(VERSION 3.16)

project(qt_vscode VERSION 0.1 LANGUAGES CXX)

# Print current platform name
message(STATUS "Current platform: ${CMAKE_SYSTEM_NAME}, UNIX: ${UNIX}, APPLE: ${APPLE}, WIN32: ${WIN32}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_FILENAME qt_vscode)

# MSVC 下添加 /Zc:__cplusplus
# if(MSVC)
# add_compile_options(/Zc:__cplusplus)
# add_compile_options(/permissive-)
# endif()

# 根据平台选择 Qt 版本
if(UNIX AND NOT APPLE)
    # Linux 环境使用 Qt5
    set(QT_VERSION_MAJOR 5)
    message(STATUS "Detected Linux platform, using Qt 5")
    find_package(Qt5 REQUIRED COMPONENTS Widgets)
elseif(WIN32)
    # Windows 环境使用 Qt6
    set(QT_VERSION_MAJOR 6)
    message(STATUS "Detected Windows platform, using Qt 6")
    # Set CMAKE_PREFIX_PATH to Qt6 installation directory
    # Note: Adjust the path to match your actual Qt6 installation location
    set(CMAKE_PREFIX_PATH "D:\\AppsDev\\Qt\\6.9.2\\msvc2022_64")
    find_package(Qt6 REQUIRED COMPONENTS Widgets)
    qt_standard_project_setup()
else()
    # 其他平台不支持
    message(FATAL_ERROR "Unsupported platform")
endif()

# 包含 src 目录
add_subdirectory(src)


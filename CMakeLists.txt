cmake_minimum_required(VERSION 3.16)

project(qt_vscode VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EXECUTABLE_FILENAME qt_vscode)

# MSVC 下添加 /Zc:__cplusplus
# if(MSVC)
#     add_compile_options(/Zc:__cplusplus)
#     add_compile_options(/permissive-)
# endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

add_subdirectory(src)
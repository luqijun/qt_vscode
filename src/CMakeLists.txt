# Detect platform and set Qt version accordingly
if(UNIX AND NOT APPLE)
    # Process UI file for Qt 5
    qt5_wrap_ui(UIC_HEADERS mainwindow.ui)

    # Process MOC files for Qt 5
    qt5_wrap_cpp(MOC_SOURCES mainwindow.h)

    add_executable(${EXECUTABLE_FILENAME}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        ${UIC_HEADERS}
        ${MOC_SOURCES}
    )

    # Add build directory to include path for generated UI headers
    target_include_directories(${EXECUTABLE_FILENAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    # Find Python3
    set(Python3_ROOT_DIR "/home/numbat/miniconda3/envs/py38")
    set(PYTHON_EXECUTABLE "/home/numbat/miniconda3/envs/py38/bin/python")
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

elseif(WIN32)
    # Process UI file for Qt 6
    qt_add_executable(${EXECUTABLE_FILENAME}
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
    )

    # Find Python3
    set(Python3_ROOT_DIR "D:\\AppsDev\\Python38")
    set(PYTHON_EXECUTABLE "D:\\AppsDev\\Python38\\python.exe")
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

else()
    message(FATAL_ERROR "Unsupported platform")
endif()



# Add Python include directories
target_include_directories(${EXECUTABLE_FILENAME} PRIVATE ${Python3_INCLUDE_DIRS})
# target_link_libraries(${EXECUTABLE_FILENAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(${EXECUTABLE_FILENAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    ${Python3_LIBRARIES}  # 使用变量替代目标
)

set_target_properties(${EXECUTABLE_FILENAME} PROPERTIES
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS ${EXECUTABLE_FILENAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

message(STATUS "=====CMAKE_INSTALL_LIBDIR:" ${CMAKE_INSTALL_LIBDIR})
message(STATUS "=====CMAKE_INSTALL_BINDIR:" ${CMAKE_INSTALL_BINDIR})
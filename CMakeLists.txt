cmake_minimum_required(VERSION 3.17)

project(networks_lab_3 CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 20)
set(QT_PATH /Users/theevilroot/Development/Qt/5.13.2/clang_64/)
set(CMAKE_PREFIX_PATH ${QT_PATH})

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES main.cc main.ui)
qt5_wrap_ui(SOURCES main.ui)

add_executable(${PROJECT_NAME} MACOSX_BUNDLE ${SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${QT_PATH}bin/macdeployqt ${PROJECT_NAME}.app)

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

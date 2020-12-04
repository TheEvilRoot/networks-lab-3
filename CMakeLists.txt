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
set(CMAKE_PREFIX_PATH ~/Development/Qt/5.13.2/clang_64/)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(SOURCES main.cc main.ui)
qt5_wrap_ui(SOURCES main.ui)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)
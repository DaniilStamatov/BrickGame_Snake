cmake_minimum_required(VERSION 3.5)

project(snake_project LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(PROJECT_SOURCES
    ../brick_game/snake/model/snake.cc
    ../brick_game/snake/model/snake.h
    ../brick_game/snake/controller/controller.cc
    ../brick_game/snake/controller/controller.h
    ../brick_game/snake/model/game.h
    ../brick_game/snake/model/game.cc
    ../gui/desktop/window.cc
    ../gui/desktop/window.h
    ../main_qt.cc
)
add_definitions(-DSNAKE)
add_executable(snake_desktop ${PROJECT_SOURCES})

target_link_libraries(snake_desktop Qt${QT_VERSION_MAJOR}::Widgets)
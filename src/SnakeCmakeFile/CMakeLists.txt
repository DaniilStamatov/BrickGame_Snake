cmake_minimum_required(VERSION 3.5)

project(snake_project LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
# Поиск необходимых модулей Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
# Указание исходных файлов проекта
set(PROJECT_SOURCES
    ../brick_game/snake/model/snake.cpp
    ../brick_game/snake/model/snake.h
    ../brick_game/snake/controller/controller.cpp
    ../brick_game/snake/controller/controller.h
    ../brick_game/snake/model/game.h
    ../brick_game/snake/model/game.cpp
    ../gui/desktop/window.cpp
    ../gui/desktop/window.h
    ../main_qt.cpp
)
add_definitions(-DSNAKE)
# Создание исполняемого файла
add_executable(MyExecutable ${PROJECT_SOURCES})

# Связывание с библиотеками Qt
target_link_libraries(MyExecutable Qt${QT_VERSION_MAJOR}::Widgets)
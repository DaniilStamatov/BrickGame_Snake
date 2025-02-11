cmake_minimum_required(VERSION 3.5)

project(snake_project LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
# Поиск необходимых модулей Qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set_source_files_properties(
    ../brick_game/tetris/tetris_controller.c
    ../brick_game/tetris/tetris_game.c
    ../brick_game/tetris/tetris_matrix_init.c
    ../brick_game/tetris/tetris_state_machine.c
    PROPERTIES LANGUAGE C
)
# Указание исходных файлов проекта
set(PROJECT_SOURCES
    ../brick_game/tetris/tetris_controller.c
    ../brick_game/tetris/tetris_game.c
    ../brick_game/tetris/tetris_matrix_init.c
    ../brick_game/tetris/tetris_state_machine.c
    ../brick_game/common.h
    ../brick_game/tetris/tetris.h
    ../gui/desktop/window.cpp
    ../gui/desktop/window.h
    ../main_qt.cpp
)
add_definitions(-DTETRIS)
# Создание исполняемого файла
add_executable(MyExecutable ${PROJECT_SOURCES})

# Связывание с библиотеками Qt
target_link_libraries(MyExecutable Qt${QT_VERSION_MAJOR}::Widgets)  

target_compile_options(MyExecutable PRIVATE
    $<$<COMPILE_LANGUAGE:CXX>:-g>
    $<$<COMPILE_LANGUAGE:C>:-g>
)
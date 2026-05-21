 cmake_minimum_required(VERSION 3.16)
project(simple_notepad_project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui)

add_executable(Notepad
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        spell_highlighter.cpp
        spell_highlighter.h
        notepad_exception.h
)

target_link_libraries(Notepad PRIVATE Qt6::Widgets Qt6::Gui)
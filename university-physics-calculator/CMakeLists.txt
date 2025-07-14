cmake_minimum_required(VERSION 3.16)

project(PhysCalc CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

include_directories(include lib)

add_executable(phys_calc)

set(PROJECT_SOURCES
    src/DataSet.cpp
    src/Calculator.cpp
    src/MainWindow.cpp
    include/MainWindow.h
    src/ReportGenerator.cpp
)

target_sources(phys_calc PRIVATE
    src/main.cpp
    ${PROJECT_SOURCES}
)

target_sources(phys_calc PRIVATE
    ui/mainwindow.ui
)

target_link_libraries(phys_calc PRIVATE Qt6::Widgets)
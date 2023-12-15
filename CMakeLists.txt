# cmake_minimum_required(VERSION 3.23)
project(PROJ_DA_1)

set(CMAKE_CXX_STANDARD 14)

add_executable(PROJ_DA_1 src/main.cpp src/graph.h src/utils/csv_reader.h src/utils/csv_reader.cpp src/manager.h src/manager.cpp src/graph.cpp src/ford_fulkerson.cpp src/menu/menu.h src/menu/menu.cpp)

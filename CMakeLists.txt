cmake_minimum_required(VERSION 3.14)
project(database)

find_package(Boost REQUIRED COMPONENTS system)

add_subdirectory(src/server)

add_executable(database src/main.cpp)

target_link_libraries(database Boost::system server)

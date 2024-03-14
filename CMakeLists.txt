cmake_minimum_required(VERSION 3.14)
project(database)

find_package(Boost REQUIRED COMPONENTS system)
find_package(TBB REQUIRED)

add_subdirectory(src/server)
add_subdirectory(src/utils)
add_subdirectory(src/execution)
add_subdirectory(src/persistence)

add_executable(database src/main.cpp)

set_target_properties(database PROPERTIES CXX_STANDARD 20)

target_link_libraries(database Boost::system TBB::tbb server utils execution persistence)

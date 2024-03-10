cmake_minimum_required(VERSION 3.14)

project(execution)

add_library(execution STATIC
  server.hpp
  server.cpp
)

set_target_properties(execution PROPERTIES CXX_STANDARD 17)

target_include_directories(
  execution
  PUBLIC .
)

target_link_libraries(server Boost::system)

cmake_minimum_required(VERSION 3.14)

project(server)

add_library(server STATIC
  server.hpp
  server.cpp
)

set_target_properties(server PROPERTIES CXX_STANDARD 20)

target_include_directories(
  server
  PUBLIC .
  ../execution
  ../persistence
)

target_link_libraries(server Boost::system)

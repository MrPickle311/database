cmake_minimum_required(VERSION 3.14)

project(server)

add_library(server STATIC
  server.hpp
  server.cpp
)

set_target_properties(server PROPERTIES CXX_STANDARD 17)

target_include_directories(
  server
  PUBLIC .
)

target_link_libraries(server Boost::system)

cmake_minimum_required(VERSION 3.14)

project(execution)

add_library(execution STATIC
  command.hpp
  command.cpp
  parser.hpp
  parser.cpp
)

set_target_properties(execution PROPERTIES CXX_STANDARD 20)

target_include_directories(
  execution
  PUBLIC .
  ../persistence
  ../utils
)

target_link_libraries(server Boost::system)

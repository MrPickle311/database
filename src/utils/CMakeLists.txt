cmake_minimum_required(VERSION 3.14)

project(utils)

add_library(utils STATIC
  utils.hpp
  utils.cpp
)

set_target_properties(utils PROPERTIES CXX_STANDARD 17)

target_include_directories(
  utils
  PUBLIC .
)

target_link_libraries(utils Boost::system)

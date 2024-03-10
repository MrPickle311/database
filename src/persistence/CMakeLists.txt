cmake_minimum_required(VERSION 3.14)

project(repository)

add_library(repository STATIC
  repository.hpp
  repository.cpp
)

set_target_properties(repository PROPERTIES CXX_STANDARD 20)

target_include_directories(
  repository
  PUBLIC .
)

target_link_libraries(repository Boost::system)

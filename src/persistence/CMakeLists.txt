cmake_minimum_required(VERSION 3.14)

project(persistence)

add_library(persistence STATIC
  repository.hpp
  repository.cpp
)

set_target_properties(persistence PROPERTIES CXX_STANDARD 20)

target_include_directories(
  persistence
  PUBLIC .
)

target_link_libraries(persistence Boost::system TBB::tbb)

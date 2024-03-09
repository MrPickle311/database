cmake_minimum_required(VERSION 3.14)

project(application)

add_library(application STATIC
  application.hpp
)

target_include_directories(
  application
  PUBLIC .
)

target_link_libraries(application Boost::system)

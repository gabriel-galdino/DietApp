cmake_minimum_required(VERSION 3.1)

add_library(application
  application.cc
)
target_include_directories (application PRIVATE
  ${CMAKE_SOURCE_DIR}/include)
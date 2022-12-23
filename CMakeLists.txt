cmake_minimum_required(VERSION 3.20)

project(
  parser-combinator
  VERSION 0.0.1
  DESCRIPTION "A pretty cool parser combinator library."
  LANGUAGES CXX
)

option(BUILD_TESTS "Also build all the tests" OFF)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

add_subdirectory(src)

if(BUILD_TESTS)
  add_subdirectory(test)
endif()

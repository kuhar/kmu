cmake_minimum_required(VERSION 2.6)
project(kmu)

add_definitions(-std=c++1y -Wall -Werror -Wextra -pedantic)
add_executable(kmu src/Examples/Examples.cpp)

set(MainDirectory src/)
include_directories(${MainDirectory})
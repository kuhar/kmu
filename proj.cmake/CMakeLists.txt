cmake_minimum_required(VERSION 2.6)
project(kmu)

add_definitions(-std=c++1y -stdlib=libstdc++ -Wall -Werror -Wextra -pedantic)
add_executable(kmu ../Examples/Examples.cpp)

set(MainDirectory ../)
include_directories(${MainDirectory})
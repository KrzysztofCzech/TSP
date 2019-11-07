cmake_minimum_required(VERSION 3.13)
project(komiwojazer_problem)

set(CMAKE_CXX_STANDARD 14)

include_directories(
        include
)

add_executable(komiwojazer_problem main.cpp include/tsp.hpp src/tsp.cpp include/bhh.hpp)
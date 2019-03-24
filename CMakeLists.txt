cmake_minimum_required(VERSION 3.8)
project(Project_1)
set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp process.h instruction.h process.cpp instruction.cpp)

add_executable(Project_1 ${SOURCE_FILES})
cmake_minimum_required(VERSION 3.1.0)
project(PiHWCtrl)

# Define that we want to use C++14
set (CMAKE_CXX_STANDARD 14)

# Use the include directory for the header files
include_directories(include)

# Generate a library from all files under src/lib
file(GLOB_RECURSE SOURCES "src/lib/*.cpp")
add_library(pihwctrl SHARED ${SOURCES})

add_executable(mytest src/program/test.cpp)
target_link_libraries(mytest pihwctrl pigpio pthread)


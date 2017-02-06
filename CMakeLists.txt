cmake_minimum_required(VERSION 3.1.0)
project(PiHWCtrl)

# Define that we want to use C++14
set (CMAKE_CXX_STANDARD 14)

# Use the include directory for the header files
include_directories(include)

# Generate a library from all files under src/lib
file(GLOB_RECURSE SOURCES "src/lib/*.cpp")
add_library(pihwctrl SHARED ${SOURCES})

####################################
# Generate the example executables #
####################################

# A function to simplify the calls
function(add_example example_name)
  add_executable(${example_name} src/examples/${example_name}.cpp)
  target_link_libraries(${example_name} pihwctrl pigpio pthread)
endfunction()

add_example(PigpioBinaryInputExample)
add_example(FunWithLEDs)

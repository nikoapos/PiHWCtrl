cmake_minimum_required(VERSION 3.1.0)
project(PiHWCtrl)

#########################
# Build the C++ library #
#########################

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
add_example(PigpioSwitchExample)
add_example(FunWithLEDs)
add_example(PigpioPWMExample)
add_example(FunWithLEDsPWM)
add_example(BMP180Example)


###########################
# Build the SWIG bindings #
###########################

## Find the SWIG and run its cmake file 
#find_package(SWIG REQUIRED)
#include(${SWIG_USE_FILE})
#
## Add the Python headers in the include directories
#find_package(PythonLibs REQUIRED)
#include_directories(${PYTHON_INCLUDE_PATH})
#
#set_source_files_properties(swig/Bindings.i PROPERTIES CPLUSPLUS ON)
#set_source_files_properties(swig/Bindings.i SWIG_FLAGS "-includeall")
#swig_add_module(PiHWCtrl python swig/Bindings.i)
#swig_link_libraries(PiHWCtrl pihwctrl pigpio pthread ${PYTHON_LIBRARIES})
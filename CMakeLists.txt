cmake_minimum_required(VERSION 3.1.0)
project(PiHWCtrl)

####################################
# Set the directories of the build #
####################################
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(PYTHON_OUTPUT_PATH ${PROJECT_BINARY_DIR}/python)

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

file(GLOB EXAMPLE_FILES "src/examples/*.cpp")
foreach(example_file ${EXAMPLE_FILES})
    get_filename_component(example_name ${example_file} NAME_WE)
    add_executable(${example_name} ${example_file})
    target_link_libraries(${example_name} pihwctrl pigpio pthread)
endforeach(example_file)


###########################
# Build the SWIG bindings #
###########################

# Find the SWIG and run its cmake file 
find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# Add the Python headers in the include directories
find_package(PythonLibs REQUIRED)
include_directories(${PYTHON_INCLUDE_PATH})

# Set the path in the build directory to create everything
set(CMAKE_SWIG_OUTDIR ${PYTHON_OUTPUT_PATH}/PiHWCtrl)
file(WRITE ${PYTHON_OUTPUT_PATH}/PiHWCtrl/__init__.py)

# Add all the modules
file(GLOB SWIG_FILES "swig/*.i")
foreach(swig_file ${SWIG_FILES})
    get_filename_component(module_name ${swig_file} NAME_WE)
    set_source_files_properties($swig_file} PROPERTIES CPLUSPLUS ON)
    swig_add_module(${module_name} python ${swig_file})
    swig_link_libraries(${module_name} pihwctrl pigpio pthread ${PYTHON_LIBRARIES})
endforeach(swig_file)

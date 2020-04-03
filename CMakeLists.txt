cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(word2vec LANGUAGES CXX C)

# Sets variable to a value if variable is unset.
macro(set_ifndef var val)
    if(NOT ${var})
        set(${var} ${val})
    endif()
    message(STATUS "Configurable variable ${var} set to ${${var}}")
endmacro()

# -------- CONFIGURATION --------
# Set module name here. MUST MATCH the module name specified in the .cpp
set_ifndef(PY_MODULE_NAME word2vec)
# Set C++11 as standard for the whole project
set(CMAKE_CXX_STANDARD 11)
# pybind11 defaults to c++14.
set(PYBIND11_CPP_STANDARD -std=c++11)
set_ifndef(PYBIND11_DIR pybind11/)
# Find dependencies.
message("\nThe following variables are derived from the values of the previous variables unless provided explicitly:\n")



find_path(_PYTHON2_INC_DIR Python.h HINTS ${PYTHON_ROOT} PATH_SUFFIXES python2.7)
set_ifndef(PYTHON2_INC_DIR ${_PYTHON2_INC_DIR})

find_path(_PYTHON3_INC_DIR Python.h HINTS ${PYTHON_ROOT} PATH_SUFFIXES  python3.7 python3.6 python3.5)
set_ifndef(PYTHON3_INC_DIR ${_PYTHON3_INC_DIR})

# -------- BUILDING --------



# Add this so we can retrieve pybind11_add_module.
add_subdirectory(${PYBIND11_DIR} ${CMAKE_BINARY_DIR}/pybind11)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/plugin/*.cpp)

# Bindings library. The module name MUST MATCH the module name specified in the .cpp
if(PYTHON3_INC_DIR AND NOT (${PYTHON3_INC_DIR} STREQUAL "None"))
    pybind11_add_module(${PY_MODULE_NAME} SHARED THIN_LTO ${SOURCE_FILES})
    target_include_directories(${PY_MODULE_NAME} BEFORE PUBLIC ${PYTHON3_INC_DIR})
    set_target_properties(${PY_MODULE_NAME} PROPERTIES OUTPUT_NAME ${PY_MODULE_NAME} SUFFIX ".so")
    install(TARGETS ${PY_MODULE_NAME} LIBRARY DESTINATION embedding)
    
endif()

if(PYTHON2_INC_DIR AND NOT (${PYTHON2_INC_DIR} STREQUAL "None"))
    # Suffix the cmake target name with a 2 to differentiate from the Python 3 bindings target.
    pybind11_add_module(${PY_MODULE_NAME}2 SHARED THIN_LTO ${SOURCE_FILES})
    target_include_directories(${PY_MODULE_NAME}2 BEFORE PUBLIC ${PYTHON2_INC_DIR})
    # Rename to remove the .cpython-35... extension.
    set_target_properties(${PY_MODULE_NAME}2 PROPERTIES OUTPUT_NAME ${PY_MODULE_NAME} SUFFIX ".so")
    # Python 2 requires an empty __init__ file to be able to import.
    file(WRITE ${CMAKE_BINARY_DIR}/__init__.py "")
    install(TARGETS ${PY_MODULE_NAME} LIBRARY DESTINATION embedding)
endif()

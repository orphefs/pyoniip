cmake_minimum_required(VERSION 3.4...3.18)
project(pyoniip)

add_subdirectory(pybind11)
set(CMAKE_CXX_STANDARD 17)
pybind11_add_module(pyoniip src/main.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(pyoniip
                           PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})

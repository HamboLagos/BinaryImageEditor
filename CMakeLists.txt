# Set up global definitions
cmake_minimum_required(VERSION 3.5)
project(BinaryImageEditor)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 11)

# The primary cmakelists is in src/
add_subdirectory(src)

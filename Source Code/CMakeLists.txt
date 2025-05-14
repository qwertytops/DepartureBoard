cmake_minimum_required(VERSION 3.29.1)

# Project name
project(DepartureBoard)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(includes)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Create the executable
add_executable(DepartureBoard ${SOURCES})

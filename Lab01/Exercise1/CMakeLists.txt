cmake_minimum_required(VERSION 3.20)

# Project name and version
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(my_program main.cpp)

# Optional: Enable debugging symbols
set(CMAKE_BUILD_TYPE Debug -pthread)

# Include directories (if needed)
# include_directories(${CMAKE_SOURCE_DIR}/include)

# Link external libraries (if needed)
# target_link_libraries(my_program PRIVATE my_library)

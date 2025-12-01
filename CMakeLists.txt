#minimum cMake version for build process
cmake_minimum_required (VERSION 3.28)

# I set PROJECT_NAME variable to example1
set(PROJECT_NAME example1)

# Set C++ standard 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project and tell cMake it is a C/C++ project
project(${PROJECT_NAME} CXX)

# Set different output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin")

# Get all source files recursively and store it into SOURCES variable
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp")

# Add the executable with all found sources in SOURCES
add_executable(${PROJECT_NAME} ${SOURCES})

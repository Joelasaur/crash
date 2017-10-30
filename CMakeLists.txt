
# Use "-DCMAKE_BUILD_TYPE=Debug" argument to build for gdb debugging

cmake_minimum_required(VERSION 2.6)
project(crash CXX)

# Locate GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
 
# Link runTests with what we want to test and the GTest and pthread library
add_executable(runTests tests.cpp)
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

add_executable(crash main.cpp crash.cpp)

# Set C++ compiler 
SET(CMAKE_CXX_FLAGS "-std=c++11")
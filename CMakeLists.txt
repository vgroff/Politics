cmake_minimum_required(VERSION 2.8)

project (MYGAME)
set (EXECUTABLE_NAME "Politics")
set (CMAKE_CXX_COMPILER "/usr/bin/g++-5")
set (CMAKE_CXX_STANDARD 11)
include_directories("include/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_BUILD_TYPE Debug)
add_executable(politics "main.cpp")

add_subdirectory("include")
add_subdirectory("src")

list(APPEND PUBLIC_INCLUDES $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
list(APPEND PUBLIC_INCLUDES $<INSTALL_INTERFACE:include>)
target_include_directories(politics PUBLIC ${PUBLIC_INCLUDES})


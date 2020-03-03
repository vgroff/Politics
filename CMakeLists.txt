cmake_minimum_required(VERSION 2.8)

project (MYGAME)
set (EXECUTABLE_NAME "Politics")

add_executable(politics "main.cpp")

add_subdirectory("include")
add_subdirectory("src")


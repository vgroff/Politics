cmake_minimum_required(VERSION 2.8)

project (MYGAME)
set (EXECUTABLE_NAME "Politics")
set (CMAKE_CXX_COMPILER "clang++")
set (CMAKE_CXX_STANDARD 17)
include_directories("include/")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_BUILD_TYPE Debug)
add_executable(politics "main.cpp")

add_subdirectory("include")
add_subdirectory("src")

list(APPEND PUBLIC_INCLUDES $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>)
list(APPEND PUBLIC_INCLUDES $<INSTALL_INTERFACE:include>)
target_include_directories(politics PUBLIC ${PUBLIC_INCLUDES})

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.71.0) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(politics ${Boost_LIBRARIES})
endif()

add_custom_target(run "./politics")

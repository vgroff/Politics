cmake_minimum_required(VERSION 2.8)

project (MYGAME)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set (CMAKE_INCLUDE_CURRENT_DIR ON)
set (EXECUTABLE_NAME "Politics")
set (CMAKE_CXX_COMPILER "clang++")
set (CMAKE_CXX_STANDARD 17)
include_directories("include/" "usr/lib")
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

set(Qt5_DIR "/usr/local/opt/qt/lib/cmake/") 
find_package(Qt5 HINTS "${Qt5_DIR}" COMPONENTS Core Widgets REQUIRED)

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
endif()

target_link_libraries(politics ${Boost_LIBRARIES} Qt5::Widgets)


add_custom_target(run "./politics")

# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(VMT CXX)
# Void Modding Tool

set(CMAKE_CXX_STANDARD 17) # because we can
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)

add_executable(VMTTool ${SOURCES})

if(WIN32) # linux users have WINE, lol
    target_link_libraries(VMTTool PRIVATE user32 gdi32)
endif()

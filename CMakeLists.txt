cmake_minimum_required(VERSION 3.0)

project(uart)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmax-errors=5")

set(CMAKE_BUILD_TYPE Debug)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/bu04_handler.cpp
) 

include_directories(${CMAKE_SOURCE_DIR}/include)
add_executable(uart ${CMAKE_SOURCE_DIR}/src/main.cpp ${SOURCES})



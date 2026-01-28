cmake_minimum_required(VERSION 3.16)

project(search_engine)

set(CMAKE_CXX_STANDARD 17)

set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(RESOURCES ${CMAKE_SOURCE_DIR}/src)

add_subdirectory(src )
add_subdirectory(test)

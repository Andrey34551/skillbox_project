cmake_minimum_required(VERSION 3.16)

project(search_engine_tests)

add_subdirectory(googletest-main nlohmann_json)

add_executable(search_engine_tests
    test.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/ConverterJSON.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/InvertedIndex.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../src/SearchServer.cpp

)

enable_testing()

target_include_directories(search_engine_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
    ${CMAKE_CURRENT_SOURCE_DIR}/../src
)
if(TARGET nlohmann_json)
    target_link_libraries(search_engine_tests PRIVATE nlohmann_json)
endif()

if(TARGET gtest AND TARGET gtest_main)
    target_link_libraries(search_engine_tests PRIVATE gtest gtest_main)
endif()



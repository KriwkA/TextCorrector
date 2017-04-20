cmake_minimum_required(VERSION 2.6)

project(TextCorrector)

set (CMAKE_CXX_STANDARD 11)
add_subdirectory(wordbook)
include_directories (wordbook)

add_executable(TextCorrector main.cpp text.cpp)
target_link_libraries(TextCorrector wordbook)



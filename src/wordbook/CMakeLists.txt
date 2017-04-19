cmake_minimum_required(VERSION 2.6)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_library(wordbook wordbook.h wordbook_p.h wordbook.cpp)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_library(wordbookd wordbook.h wordbook_p.h wordbook.cpp)
endif()




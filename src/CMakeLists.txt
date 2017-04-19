cmake_minimum_required(VERSION 2.6)

project(TextCorrector)

add_subdirectory(wordbook)
include_directories (wordbook)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(TextCorrector main.cpp text.cpp)
    target_link_libraries(TextCorrector wordbook)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")   
    add_executable(TextCorrectord main.cpp text.cpp)
    target_link_libraries(TextCorrectord wordbookd)
endif()




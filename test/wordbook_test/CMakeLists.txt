cmake_minimum_required(VERSION 2.6)


include_directories (../../src/wordbook)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_executable(wordbook_test wordbook_test.cpp)
    target_link_libraries(wordbook_test wordbook)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_executable(wordbook_testd wordbook_test.cpp)
    target_link_libraries(wordbook_testd wordbookd)
endif()




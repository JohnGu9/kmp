cmake_minimum_required(VERSION 3.10)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    project(
        kmp
        LANGUAGES C)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS ON)

    if(MSVC)
        add_compile_options(/Wall)
    else()
        add_compile_options(-Wall)
    endif()
endif()

set(kmp_src ${CMAKE_CURRENT_LIST_DIR}/kmp.c)
add_library(kmp OBJECT ${kmp_src})
target_include_directories(kmp PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(MSVC)
else()
    target_compile_options(kmp PRIVATE -Wextra -Wshadow)
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    add_executable(kmp_test ${CMAKE_CURRENT_LIST_DIR}/test.c)
    target_link_libraries(kmp_test PRIVATE kmp)
endif()

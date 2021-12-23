cmake_minimum_required(VERSION 3.11)
project(GAME)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(MSVC)
    add_compile_options("/std:c++17" "/W4" "$<$<CONFIG:RELEASE>:/O2>")
else()
    add_compile_options("-std=c++17" "-Wall" "-Wextra" "-Wshadow" "-pedantic" "$<$<CONFIG:RELEASE>:-O3>")
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options("-stdlib=libc++")
    else()
        # nothing special for gcc at the moment
    endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_subdirectory(src)

enable_testing()
add_subdirectory(test)

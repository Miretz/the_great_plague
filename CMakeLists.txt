cmake_minimum_required(VERSION 3.17)
project(the_great_plague)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -ggdb -Wall -Wextra -Wshadow -pedantic")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

set(SOURCES 
    src/abilities.cpp
    src/areas.cpp
    src/combat_system.cpp
    src/conversation_system.cpp
    src/dice.cpp
    src/files.cpp
    src/character_creator.cpp
    src/characters.cpp
    src/inventory_manager.cpp
    src/main.cpp
    src/utils.cpp
)

add_executable(rpg ${SOURCES})

target_include_directories(rpg PUBLIC include)

add_custom_command(TARGET rpg PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:rpg>)


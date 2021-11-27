set(SOURCES
    entities.hpp
    
    abilities.hpp
    abilities.cpp
    
    areas.hpp
    areas.cpp

    combat_system.hpp
    combat_system.cpp

    conversation_system.hpp
    conversation_system.cpp

    dice.hpp
    dice.cpp

    files.hpp
    files.cpp

    character_creator.hpp
    character_creator.cpp

    characters.hpp
    characters.cpp

    inventory_manager.hpp
    inventory_manager.cpp

    utils.hpp
    utils.cpp
)

add_library(game_lib ${SOURCES})
add_executable(the_great_plague main.cpp)
target_link_libraries(the_great_plague game_lib)

add_custom_command(TARGET the_great_plague POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${GAME_SOURCE_DIR}/assets/ $<TARGET_FILE_DIR:the_great_plague>/assets/)

#pragma once
#ifndef areas_hpp
#define areas_hpp

#include <functional>
#include <string>
#include <vector>


struct GameState;

namespace Areas
{
    const std::string AREA_FOLDER = "assets/areas/";
    const std::string DESC_FILE = "/description.txt";
    const std::string IMAGE_FILE = "/image.txt";

    const std::string f_danseaConfig = "assets/characters/dansea/dansea.txt";
    const std::string f_danseaPicture = "assets/characters/dansea/dansea_image.txt";
    const std::string f_danseaIntro = "assets/characters/dansea/dansea_introduction.txt";

    struct Area
    {
        std::string name;
        std::string folder;
        std::vector<uint32_t> connections;
        std::function<void(GameState &)> auxFunction;
    };

    void navigateToArea(uint32_t areaId, GameState &gameState);

    void t01_shore_DanseaConversation(GameState &gameState);

    void t03_gateCombat(GameState &gameState);

    void t04_inkeeperConversation(GameState &gameState);

    const std::vector<Area> allAreas{ { "Introduction", "00_introduction", { 1 }, nullptr },
                                      { "Shore", "01_shore", { 2 }, t01_shore_DanseaConversation },
                                      { "Forest", "02_forest", { 3 }, nullptr },
                                      { "City Gate", "03_city_gate", { 4, 5 }, t03_gateCombat },
                                      { "The Inn", "04_inn", { 3, 5 }, t04_inkeeperConversation },
                                      { "Town Square", "05_town_square", { 3, 4 }, nullptr } };
}  // namespace Areas

#endif
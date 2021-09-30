#pragma once
#ifndef areas_hpp
#define areas_hpp

#include <string>
#include <vector>
#include <functional>

struct GameState;

namespace Areas
{

    const std::string f_t00_introduction = "assets/areas/t00_introduction.txt";
    const std::string f_t01_shore = "assets/areas/t01_shore.txt";
    const std::string f_t02_forest = "assets/areas/t02_forest.txt";
    const std::string f_t03_cityGate = "assets/areas/t02_city_gate.txt";

    const std::string f_danseaPicture = "assets/characters/dansea.txt";
    const std::string f_danseaIntro = "assets/characters/dansea_introduction.txt";

    struct Area
    {
        std::string name;
        std::string descriptionFile;
        std::vector<uint32_t> connections;
        std::function<void(GameState &)> auxFunction;
    };

    void navigateToArea(uint32_t areaId, GameState &game);

    void t01_shore_DanseaConversation(GameState &game);

    const std::vector<Area> allAreas{
        {"Introduction", f_t00_introduction, {1}, nullptr},
        {"Shore", f_t01_shore, {2}, t01_shore_DanseaConversation},
        {"Forest", f_t02_forest, {1, 3}, nullptr},
        {"City Gate", f_t03_cityGate, {}, nullptr}};
}

#endif
#pragma once
#ifndef areas_hpp
#define areas_hpp

#include <functional>
#include <string>
#include <vector>

struct GameState;

namespace Areas
{
    static constexpr auto kAreaFolder = "assets/areas/";
    static constexpr auto kDescriptionFile = "/description.txt";
    static constexpr auto kImageFile = "/image.txt";

    static constexpr auto kDanseaConfigFile = "assets/characters/dansea/dansea.txt";
    static constexpr auto kDanseaPictureFile = "assets/characters/dansea/dansea_image.txt";
    static constexpr auto kDanseaIntroFile = "assets/characters/dansea/dansea_introduction.txt";

    static constexpr auto kDanseaLocationGameProperty = "danseaLocation";
    static constexpr auto kDanseaConversationFile = "assets/areas/01_shore/conversation.txt";

    static constexpr auto kInnkeeperPictureFile = "assets/areas/04_inn/image.txt";
    static constexpr auto kInnkeeperConversationFile = "assets/areas/04_inn/conversation.txt";

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

    static const std::vector<Area> allAreas{
        { "Introduction", "00_introduction", { 1 }, nullptr },
        { "Shore", "01_shore", { 2 }, t01_shore_DanseaConversation },
        { "Forest", "02_forest", { 3 }, nullptr },
        { "City Gate", "03_city_gate", { 4, 5 }, t03_gateCombat },
        { "The Inn", "04_inn", { 3, 5 }, t04_inkeeperConversation },
        { "Town Square", "05_town_square", { 3, 4 }, nullptr }
    };
}  // namespace Areas

#endif
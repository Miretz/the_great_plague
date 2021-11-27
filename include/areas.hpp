#ifndef areas_hpp
#define areas_hpp

#include <functional>
#include <string>
#include <vector>

struct GameState;

namespace Areas
{
    inline constexpr auto kAreaFolder = "assets/areas/";
    inline constexpr auto kDescriptionFile = "/description.txt";
    inline constexpr auto kImageFile = "/image.txt";

    inline constexpr auto kDanseaConfigFile = "assets/characters/dansea/dansea.txt";
    inline constexpr auto kDanseaPictureFile = "assets/characters/dansea/dansea_image.txt";
    inline constexpr auto kDanseaIntroFile = "assets/characters/dansea/dansea_introduction.txt";

    inline constexpr auto kDanseaLocationGameProperty = "danseaLocation";
    inline constexpr auto kDanseaConversationFile = "assets/areas/01_shore/conversation.txt";

    inline constexpr auto kInnkeeperPictureFile = "assets/areas/04_inn/image.txt";
    inline constexpr auto kInnkeeperConversationFile = "assets/areas/04_inn/conversation.txt";

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

    static const std::vector<Area> allAreas{ { "Introduction", "00_introduction", { 1 }, nullptr },
                                             { "Shore", "01_shore", { 2 }, t01_shore_DanseaConversation },
                                             { "Forest", "02_forest", { 3 }, nullptr },
                                             { "City Gate", "03_city_gate", { 4, 5 }, t03_gateCombat },
                                             { "The Inn", "04_inn", { 3, 5 }, t04_inkeeperConversation },
                                             { "Town Square", "05_town_square", { 3, 4 }, nullptr } };
}  // namespace Areas

#endif
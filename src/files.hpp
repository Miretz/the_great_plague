#ifndef files_hpp
#define files_hpp

#include <string>
#include <string_view>
#include <vector>

struct Hero;
struct Ability;
struct GameState;
struct ConversationLine;

namespace Files
{
    inline constexpr auto kSaveFile = "save.txt";
    inline constexpr std::string_view kDelimiter = "|";
    inline constexpr std::string_view kValueDelimiter = ";";

    [[nodiscard]] auto serializeHero(const Hero &hero) -> const std::string;

    [[nodiscard]] auto deserializeHero(std::string serialized) -> Hero;

    void saveGame(const GameState &gameState);

    [[nodiscard]] auto loadGame() -> GameState;

    [[nodiscard]] auto loadFile(const std::string &filePath) -> const std::string;

    void saveFile(const std::string &data, const std::string &filePath);

    [[nodiscard]] auto loadHeroFromConfig(const std::string &filePath) -> Hero;

    void saveHeroToFile(const Hero &hero, const std::string &filePath);

    [[nodiscard]] auto loadAbilities(const std::string &filePath) -> std::vector<Ability>;

    [[nodiscard]] auto loadConversationFile(const std::string &conversationFilePath) -> std::vector<ConversationLine>;
}  // namespace Files

#endif
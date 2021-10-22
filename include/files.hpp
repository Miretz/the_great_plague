#pragma once
#ifndef files_hpp
#define files_hpp

#include <string>
#include <vector>

struct Hero;
struct Ability;
struct GameState;
struct ConversationLine;

namespace Files
{
    static constexpr auto kSaveFile = "save.txt";

    static const std::string kDelimiter = "|";
    static const std::string kValueDelimiter = ";";

    auto serializeHero(const Hero &hero) -> const std::string;

    auto deserializeHero(std::string serialized) -> Hero;

    void saveGame(const GameState &gameState);

    auto loadGame() -> GameState;

    auto loadFile(const std::string &filePath) -> const std::string;

    void saveFile(const std::string &data, const std::string &filePath);

    auto loadHeroFromConfig(const std::string &filePath) -> Hero;

    void saveHeroToFile(const Hero &hero, const std::string &filePath);

    auto loadAbilities(const std::string &filePath) -> std::vector<Ability>;

    auto loadConversationFile(const std::string &conversationFilePath) -> std::vector<ConversationLine>;
}  // namespace Files

#endif
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
    const std::string SAVE_FILE = "save.txt";

    const std::string delimiter = "|";
    const std::string valueDelimitter = ";";

    const std::string serializeHero(const Hero &hero);

    Hero deserializeHero(std::string serialized);

    void saveGame(const GameState &gameState);

    GameState loadGame();

    const std::string loadFile(const std::string &filePath);

    void saveFile(const std::string &data, const std::string &filePath);

    Hero loadHeroFromConfig(const std::string &filePath);

    void saveHeroToFile(const Hero &hero, const std::string &filePath);

    std::vector<Ability> loadAbilities(const std::string &filePath);

    std::vector<ConversationLine> loadConversationFile(const std::string &conversationFilePath);
}  // namespace Files

#endif
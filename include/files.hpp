#pragma once
#ifndef files_hpp
#define files_hpp

#include <string>

struct Hero;
struct GameState;

namespace Files
{
    const std::string SAVE_FILE = "save.txt";

    const std::string serializeHero(const Hero &hero);

    Hero deserializeHero(std::string serialized);

    void saveGame(const GameState &gameState);

    GameState loadGame();

    const std::string loadFile(const std::string &filePath);

    void saveFile(const std::string &data, const std::string &filePath);

    Hero loadHeroFromConfig(const std::string &filePath);

    void saveHeroToFile(const Hero &hero, const std::string &filePath);
}

#endif
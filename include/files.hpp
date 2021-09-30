#pragma once
#ifndef files_hpp
#define files_hpp

#include <string>

struct Hero;
struct GameState;

namespace Files
{
    const std::string SAVE_FILE = "save.txt";

    std::string serializeHero(Hero hero);

    Hero deserializeHero(std::string serialized);

    void saveGame(GameState &gameState);

    GameState loadGame();

    std::string loadFile(const std::string &filePath);
}

#endif
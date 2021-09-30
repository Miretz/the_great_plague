#pragma once

#include "entities.hpp"

namespace Files
{
    const std::string SAVE_FILE = "save.txt";

    std::string serializeHero(Hero hero);

    Hero deserializeHero(std::string serialized);

    void saveGame(GameState &gameState);

    GameState loadGame();

    std::string loadFile(const std::string& filePath);
}
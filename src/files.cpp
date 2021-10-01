#include "files.hpp"

#include "entities.hpp"

#include <sstream>
#include <fstream>

namespace Files
{
    const std::string serializeHero(const Hero &hero)
    {
        std::ostringstream ss;

        std::string delimiter = "|";

        ss << hero.name << delimiter;

        ss << hero.health << delimiter;
        ss << hero.xp << delimiter;
        ss << hero.maxHealth << delimiter;
        ss << hero.level << delimiter;
        ss << hero.xpToLevelUp << delimiter;
        ss << hero.unspentPoints << delimiter;

        ss << static_cast<uint32_t>(hero.race) << delimiter;
        ss << static_cast<uint32_t>(hero.controller) << delimiter;

        ss << hero.attributes.strength << delimiter;
        ss << hero.attributes.dexterity << delimiter;
        ss << hero.attributes.vitality << delimiter;
        ss << hero.attributes.intelligence << delimiter;

        for (auto a : hero.abilities)
        {
            ss << a << ";";
        }
        ss << delimiter;

        for (auto b : hero.inventory.backpack)
        {
            ss << b << ";";
        }
        ss << delimiter;

        for (auto a : hero.inventory.equipped)
        {
            ss << a.first << ";" << a.second << ";";
        }
        ss << delimiter;

        return ss.str();
    }

    Hero deserializeHero(std::string serialized)
    {
        std::string delimiter = "|";

        Hero hero;

        hero.name = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.health = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.xp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.maxHealth = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.level = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.xpToLevelUp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.unspentPoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.race = static_cast<Race>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.controller = static_cast<Controller>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.strength = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.dexterity = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.vitality = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.intelligence = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        auto abilitiesStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        auto backpackStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        auto equippedStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        size_t pos = 0;
        while ((pos = abilitiesStr.find(";")) != std::string::npos)
        {
            auto token = abilitiesStr.substr(0, pos);
            if (token == "")
                break;
            hero.abilities.push_back(static_cast<uint32_t>(std::stoul(token)));
            abilitiesStr.erase(0, pos + delimiter.length());
        }

        pos = 0;
        while ((pos = backpackStr.find(";")) != std::string::npos)
        {
            auto token = backpackStr.substr(0, pos);
            if (token == "")
                break;
            hero.inventory.backpack.push_back(static_cast<uint32_t>(std::stoul(token)));
            backpackStr.erase(0, pos + delimiter.length());
        }

        pos = 0;
        while ((pos = equippedStr.find(";")) != std::string::npos)
        {
            auto key = equippedStr.substr(0, pos);
            if (key == "")
                break;
            equippedStr.erase(0, pos + delimiter.length());
            pos = equippedStr.find(";");
            auto valueStr = equippedStr.substr(0, pos);
            equippedStr.erase(0, pos + delimiter.length());
            if (valueStr == "")
                break;
            auto value = static_cast<uint32_t>(std::stoul(valueStr));
            hero.inventory.equipped[key] = value;
        }

        return hero;
    }

    void saveGame(const GameState &gameState)
    {
        std::fstream saveFile;
        saveFile.open(SAVE_FILE, std::ios::out);
        if (saveFile.is_open())
        {
            saveFile << gameState.areaId << "\n";
            saveFile << gameState.danseaLocation << "\n";
            for (auto h : gameState.heroes)
            {
                saveFile << serializeHero(h) << "\n";
            }
            saveFile.close();
        }
    }

    GameState loadGame()
    {
        GameState gs;

        std::fstream saveFile;
        saveFile.open(SAVE_FILE, std::ios::in);
        if (saveFile.is_open())
        {
            std::string line;

            getline(saveFile, line);
            gs.areaId = static_cast<uint32_t>(std::stoul(line));
            getline(saveFile, line);
            gs.danseaLocation = static_cast<uint32_t>(std::stoul(line));

            while (getline(saveFile, line))
            {
                gs.heroes.push_back(deserializeHero(line));
            }
            saveFile.close();
        }

        return gs;
    }

    const std::string loadFile(const std::string &filePath)
    {

        std::ostringstream ss;

        std::fstream file;
        file.open(filePath, std::ios::in);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                ss << line << "\n";
            }

            file.close();
        }

        return ss.str();
    }
}
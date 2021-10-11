#include "files.hpp"

#include "entities.hpp"
#include "utils.hpp"

#include <sstream>
#include <fstream>
#include <iostream>
#include <stdexcept>

namespace Files
{
    const std::string serializeHero(const Hero &hero)
    {
        std::ostringstream ss;

        ss << hero.uniqueId << delimiter;
        ss << hero.name << delimiter;

        ss << hero.health << delimiter;
        ss << hero.xp << delimiter;
        ss << hero.maxHealth << delimiter;
        ss << hero.level << delimiter;
        ss << hero.xpToLevelUp << delimiter;
        ss << hero.unspentAttributePoints << delimiter;
        ss << hero.unspentSpecialtyPoints << delimiter;

        ss << static_cast<uint32_t>(hero.race) << delimiter;
        ss << static_cast<uint32_t>(hero.controller) << delimiter;

        ss << hero.attributes.strength << delimiter;
        ss << hero.attributes.dexterity << delimiter;
        ss << hero.attributes.constitution << delimiter;
        ss << hero.attributes.intelligence << delimiter;

        ss << hero.specialties.oneHanded << delimiter;
        ss << hero.specialties.twoHanded << delimiter;
        ss << hero.specialties.ranged << delimiter;
        ss << hero.specialties.dualWielding << delimiter;

        ss << hero.specialties.mysticism << delimiter;
        ss << hero.specialties.terramancy << delimiter;
        ss << hero.specialties.hydromancy << delimiter;
        ss << hero.specialties.pyromancy << delimiter;
        ss << hero.specialties.aeromancy << delimiter;

        for (const auto &a : hero.abilities)
        {
            ss << a << valueDelimitter;
        }
        ss << delimiter;

        for (const auto &a : hero.inventory.backpack)
        {
            ss << a << valueDelimitter;
        }
        ss << delimiter;

        for (const auto &a : hero.inventory.equipped)
        {
            ss << a.first << valueDelimitter << a.second << valueDelimitter;
        }
        ss << delimiter;

        return ss.str();
    }

    Hero deserializeHero(std::string serialized)
    {
        Hero hero;

        hero.uniqueId = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

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

        hero.unspentAttributePoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.unspentSpecialtyPoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.race = static_cast<Race>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.controller = static_cast<Controller>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter)))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        // Attributes

        hero.attributes.strength = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.dexterity = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.constitution = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.attributes.intelligence = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        // Specialties

        hero.specialties.oneHanded = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.twoHanded = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.ranged = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.dualWielding = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.mysticism = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.terramancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.hydromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.pyromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        hero.specialties.aeromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(delimiter))));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        // Abilities & Inventory

        auto abilitiesStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        auto backpackStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        auto equippedStr = serialized.substr(0, serialized.find(delimiter));
        serialized.erase(0, serialized.find(delimiter) + delimiter.length());

        size_t pos = 0;
        while ((pos = abilitiesStr.find(valueDelimitter)) != std::string::npos)
        {
            auto token = abilitiesStr.substr(0, pos);
            if (token == "")
                break;
            hero.abilities.push_back(Utils::trim(token));
            abilitiesStr.erase(0, pos + valueDelimitter.length());
        }

        pos = 0;
        while ((pos = backpackStr.find(valueDelimitter)) != std::string::npos)
        {
            auto token = backpackStr.substr(0, pos);
            if (token == "")
                break;
            hero.inventory.backpack.push_back(static_cast<uint32_t>(std::stoul(token)));
            backpackStr.erase(0, pos + valueDelimitter.length());
        }

        pos = 0;
        while ((pos = equippedStr.find(valueDelimitter)) != std::string::npos)
        {
            auto key = equippedStr.substr(0, pos);
            if (key == "")
                break;
            equippedStr.erase(0, pos + valueDelimitter.length());
            pos = equippedStr.find(valueDelimitter);
            auto valueStr = equippedStr.substr(0, pos);
            equippedStr.erase(0, pos + valueDelimitter.length());
            if (valueStr == "")
                break;
            auto value = static_cast<uint32_t>(std::stoul(valueStr));
            hero.inventory.equipped[key] = value;
        }

        hero.statusEffects = {};
        hero.actionPoints = 0;

        return hero;
    }

    void saveGame(const GameState &gameState)
    {
        std::fstream file;
        file.open(SAVE_FILE, std::ios::out);
        if (file.is_open())
        {
            file << gameState.areaId << '\n';
            for (const auto &a : gameState.stateInfo)
            {
                file << a.first << valueDelimitter << a.second << valueDelimitter;
            }
            file << '\n';
            for (const auto &a : gameState.heroes)
            {
                file << serializeHero(a) << '\n';
            }
            file.close();
        }
    }

    GameState loadGame()
    {
        GameState gs;
        gs.areaId = 0;

        std::fstream file;
        file.open(SAVE_FILE, std::ios::in);
        if (file.is_open())
        {
            std::string line;

            // get the location
            getline(file, line);
            gs.areaId = static_cast<uint32_t>(std::stoul(line));
            getline(file, line);

            // extract state info to map
            size_t pos = 0;
            while ((pos = line.find(valueDelimitter)) != std::string::npos)
            {
                auto key = line.substr(0, pos);
                if (key == "")
                    break;
                line.erase(0, pos + 1);
                pos = line.find(valueDelimitter);
                auto valueStr = line.substr(0, pos);
                line.erase(0, pos + 1);
                if (valueStr == "")
                    break;
                auto value = static_cast<uint32_t>(std::stoul(valueStr));
                gs.stateInfo[key] = value;
            }

            // extract heroes
            while (getline(file, line))
            {
                gs.heroes.push_back(deserializeHero(line));
            }
            file.close();
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
                ss << line << '\n';
            }

            file.close();
        }

        return ss.str();
    }

    void saveFile(const std::string &data, const std::string &filePath)
    {
        std::fstream file;
        file.open(filePath, std::ios::out);
        if (file.is_open())
        {
            file << data << '\n';
            file.close();
        }
    }

    Hero loadHeroFromConfig(const std::string &filePath)
    {
        return deserializeHero(loadFile(filePath));
    }

    void saveHeroToFile(const Hero &hero, const std::string &filePath)
    {
        saveFile(serializeHero(hero), filePath);
    }

    std::vector<Ability> loadAbilities(const std::string &filePath)
    {
        std::vector<Ability> abilities;

        std::fstream file;
        file.open(filePath, std::ios::in);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                Ability ability;
                ability.id = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                ability.name = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                ability.description = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                ability.mapping = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                auto typeStr = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                if (typeStr == "Damage")
                {
                    ability.type = AbilityType::Damage;
                }
                else if (typeStr == "Healing")
                {
                    ability.type = AbilityType::Healing;
                }
                else if (typeStr == "Summoning")
                {
                    ability.type = AbilityType::Summoning;
                }
                else if (typeStr == "StatusEffect")
                {
                    ability.type = AbilityType::StatusEffect;
                }
                else if (typeStr == "AoE_Status")
                {
                    ability.type = AbilityType::AoE_Status;
                }
                else if (typeStr == "AoE_Damage")
                {
                    ability.type = AbilityType::AoE_Damage;
                }
                else if (typeStr == "AoE_Healing")
                {
                    ability.type = AbilityType::AoE_Healing;
                }
                else
                {
                    throw std::invalid_argument("Invalid ability type for " + ability.id + ": " + typeStr);
                }

                auto targetStr = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                if (targetStr == "Self")
                {
                    ability.target = Target::Self;
                }
                else if (targetStr == "Friendly")
                {
                    ability.target = Target::Friendly;
                }
                else if (targetStr == "Enemy")
                {
                    ability.target = Target::Enemy;
                }
                else
                {
                    throw std::invalid_argument("Invalid ability target for " + ability.id + ": " + typeStr);
                }

                auto actionPointsStr = Utils::trim(line.substr(0, line.find(valueDelimitter)));
                line.erase(0, line.find(valueDelimitter) + valueDelimitter.length());

                ability.actionPoints = static_cast<uint32_t>(std::stoul(actionPointsStr));

                abilities.push_back(ability);
            }
            file.close();
        }

        return abilities;
    }

    std::vector<ConversationLine> loadConversationFile(const std::string &conversationFilePath)
    {
        std::vector<ConversationLine> lines;

        std::fstream file;
        file.open(conversationFilePath, std::ios::in);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                ConversationLine c;

                c.label = Utils::trim(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                c.who = Utils::trim(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                c.text = Utils::trim(line.substr(0, line.find(delimiter)));
                line.erase(0, line.find(delimiter) + delimiter.length());

                auto jumpsStr = Utils::trim(line.substr(0, line.find(delimiter)));

                size_t pos = 0;
                while ((pos = jumpsStr.find(valueDelimitter)) != std::string::npos)
                {
                    auto token = jumpsStr.substr(0, pos);
                    if (token == "")
                        break;
                    c.jumps.push_back(Utils::trim(token));
                    jumpsStr.erase(0, pos + valueDelimitter.length());
                }

                lines.push_back(c);
            }
            file.close();
        }
        return lines;
    }
}
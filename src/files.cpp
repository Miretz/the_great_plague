#include "files.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "entities.hpp"
#include "utils.hpp"

namespace Files
{
    auto serializeHero(const Hero &hero) -> const std::string
    {
        std::ostringstream ss;

        ss << hero.uniqueId << kDelimiter;
        ss << hero.name << kDelimiter;

        ss << hero.health << kDelimiter;
        ss << hero.xp << kDelimiter;
        ss << hero.maxHealth << kDelimiter;
        ss << hero.level << kDelimiter;
        ss << hero.xpToLevelUp << kDelimiter;
        ss << hero.unspentAttributePoints << kDelimiter;
        ss << hero.unspentSpecialtyPoints << kDelimiter;

        ss << static_cast<uint32_t>(hero.race) << kDelimiter;
        ss << static_cast<uint32_t>(hero.controller) << kDelimiter;

        ss << hero.attributes.strength << kDelimiter;
        ss << hero.attributes.dexterity << kDelimiter;
        ss << hero.attributes.constitution << kDelimiter;
        ss << hero.attributes.intelligence << kDelimiter;

        ss << hero.specialties.oneHanded << kDelimiter;
        ss << hero.specialties.twoHanded << kDelimiter;
        ss << hero.specialties.ranged << kDelimiter;
        ss << hero.specialties.dualWielding << kDelimiter;

        ss << hero.specialties.mysticism << kDelimiter;
        ss << hero.specialties.terramancy << kDelimiter;
        ss << hero.specialties.hydromancy << kDelimiter;
        ss << hero.specialties.pyromancy << kDelimiter;
        ss << hero.specialties.aeromancy << kDelimiter;

        for (const auto &a : hero.abilities)
        {
            ss << a << kValueDelimiter;
        }
        ss << kDelimiter;

        for (const auto &a : hero.inventory.backpack)
        {
            ss << a << kValueDelimiter;
        }
        ss << kDelimiter;

        for (const auto &a : hero.inventory.equipped)
        {
            ss << a.first << kValueDelimiter << a.second << kValueDelimiter;
        }
        ss << kDelimiter;

        return ss.str();
    }

    auto deserializeHero(std::string serialized) -> Hero
    {
        Hero hero;

        hero.uniqueId = serialized.substr(0, serialized.find(kDelimiter));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.name = serialized.substr(0, serialized.find(kDelimiter));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.health = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.xp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.maxHealth = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.level = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.xpToLevelUp = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.unspentAttributePoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.unspentSpecialtyPoints = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.race = static_cast<Race>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter)))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.controller =
            static_cast<Controller>(static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter)))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        // Attributes

        hero.attributes.strength = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.attributes.dexterity = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.attributes.constitution = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.attributes.intelligence = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        // Specialties

        hero.specialties.oneHanded = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.twoHanded = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.ranged = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.dualWielding = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.mysticism = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.terramancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.hydromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.pyromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        hero.specialties.aeromancy = static_cast<uint32_t>(std::stoul(serialized.substr(0, serialized.find(kDelimiter))));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        // Abilities & Inventory

        auto abilitiesStr = serialized.substr(0, serialized.find(kDelimiter));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        auto backpackStr = serialized.substr(0, serialized.find(kDelimiter));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        auto equippedStr = serialized.substr(0, serialized.find(kDelimiter));
        serialized.erase(0, serialized.find(kDelimiter) + kDelimiter.length());

        size_t pos = 0;
        while ((pos = abilitiesStr.find(kValueDelimiter)) != std::string::npos)
        {
            auto token = abilitiesStr.substr(0, pos);
            if (token == "")
            {
                break;
            }
            hero.abilities.push_back(Utils::trim(token));
            abilitiesStr.erase(0, pos + kValueDelimiter.length());
        }

        pos = 0;
        while ((pos = backpackStr.find(kValueDelimiter)) != std::string::npos)
        {
            auto token = backpackStr.substr(0, pos);
            if (token == "")
            {
                break;
            }
            hero.inventory.backpack.push_back(static_cast<uint32_t>(std::stoul(token)));
            backpackStr.erase(0, pos + kValueDelimiter.length());
        }

        pos = 0;
        while ((pos = equippedStr.find(kValueDelimiter)) != std::string::npos)
        {
            auto key = equippedStr.substr(0, pos);
            if (key == "")
            {
                break;
            }
            equippedStr.erase(0, pos + kValueDelimiter.length());
            pos = equippedStr.find(kValueDelimiter);
            auto valueStr = equippedStr.substr(0, pos);
            equippedStr.erase(0, pos + kValueDelimiter.length());
            if (valueStr == "")
            {
                break;
            }
            auto value = static_cast<uint32_t>(std::stoul(valueStr));
            hero.inventory.equipped[key] = value;
        }

        hero.statusEffects = {};
        hero.actionPoints = 0;

        return hero;
    }

    void saveGame(const GameState &gameState)
    {
        std::ofstream file(kSaveFile);
        if (file.is_open())
        {
            file << gameState.areaId << '\n';
            for (const auto &a : gameState.stateInfo)
            {
                file << a.first << kValueDelimiter << a.second << kValueDelimiter;
            }
            file << '\n';
            for (const auto &a : gameState.heroes)
            {
                file << serializeHero(a) << '\n';
            }
            file.close();
        }
    }

    auto loadGame() -> GameState
    {
        GameState gs;
        gs.areaId = 0;

        std::ifstream file(kSaveFile);
        if (file.is_open())
        {
            std::string line;

            // get the location
            getline(file, line);
            gs.areaId = static_cast<uint32_t>(std::stoul(line));
            getline(file, line);

            // extract state info to map
            size_t pos = 0;
            while ((pos = line.find(kValueDelimiter)) != std::string::npos)
            {
                auto key = line.substr(0, pos);
                if (key == "")
                {
                    break;
                }
                line.erase(0, pos + 1);
                pos = line.find(kValueDelimiter);
                auto valueStr = line.substr(0, pos);
                line.erase(0, pos + 1);
                if (valueStr == "")
                {
                    break;
                }
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

    auto loadFile(const std::string &filePath) -> const std::string
    {
        std::ostringstream ss;

        std::ifstream file(filePath);
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
        std::ofstream file(filePath);
        if (file.is_open())
        {
            file << data << '\n';
            file.close();
        }
    }

    auto loadHeroFromConfig(const std::string &filePath) -> Hero
    {
        return deserializeHero(loadFile(filePath));
    }

    void saveHeroToFile(const Hero &hero, const std::string &filePath)
    {
        saveFile(serializeHero(hero), filePath);
    }

    auto loadAbilities(const std::string &filePath) -> std::vector<Ability>
    {
        std::vector<Ability> abilities;

        std::ifstream file(filePath);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                Ability ability;
                ability.id = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

                ability.name = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

                ability.description = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

                ability.mapping = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

                auto typeStr = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

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

                auto targetStr = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

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

                auto actionPointsStr = Utils::trim(line.substr(0, line.find(kValueDelimiter)));
                line.erase(0, line.find(kValueDelimiter) + kValueDelimiter.length());

                ability.actionPoints = static_cast<uint32_t>(std::stoul(actionPointsStr));

                abilities.push_back(ability);
            }
            file.close();
        }

        return abilities;
    }

    auto loadConversationFile(const std::string &conversationFilePath) -> std::vector<ConversationLine>
    {
        std::vector<ConversationLine> lines;

        std::ifstream file(conversationFilePath);
        if (file.is_open())
        {
            std::string line;
            while (getline(file, line))
            {
                ConversationLine c;

                c.label = Utils::trim(line.substr(0, line.find(kDelimiter)));
                line.erase(0, line.find(kDelimiter) + kDelimiter.length());

                c.who = Utils::trim(line.substr(0, line.find(kDelimiter)));
                line.erase(0, line.find(kDelimiter) + kDelimiter.length());

                c.text = Utils::trim(line.substr(0, line.find(kDelimiter)));
                line.erase(0, line.find(kDelimiter) + kDelimiter.length());

                auto jumpsStr = Utils::trim(line.substr(0, line.find(kDelimiter)));

                size_t pos = 0;
                while ((pos = jumpsStr.find(kValueDelimiter)) != std::string::npos)
                {
                    auto token = jumpsStr.substr(0, pos);
                    if (token == "")
                    {
                        break;
                    }
                    c.jumps.push_back(Utils::trim(token));
                    jumpsStr.erase(0, pos + kValueDelimiter.length());
                }

                lines.push_back(c);
            }
            file.close();
        }
        return lines;
    }
}  // namespace Files
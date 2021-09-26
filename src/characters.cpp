#include "characters.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>

namespace Characters
{
    Hero pickHeroPreset()
    {
        std::cout << "\nPlease select your hero preset\n";

        for (size_t i = 0; i < presets.size(); i++)
        {
            std::cout << "+-------------------------------------------------+\n";
            std::cout << "|" << i + 1 << ".) " << presets[i].name << "\n";
            std::cout << "+-------------------------------------------------+\n";
            printAttributes(presets[i].attributes);
            std::cout << "+-------------------------------------------------+\n";
            std::cout << "\n\n";
        }
        std::cout << "\n";

        int selection = pickOption(presets.size());
        auto hero = presets[selection - 1];

        clearScreen();

        std::cout << "\nYour choice: \n";
        std::cout << "+-------------------------------------------------+\n";
        printAttributes(hero.attributes);
        std::cout << "+-------------------------------------------------+\n";
        return hero;
    }

    Attributes updateAttributes(Attributes attributes)
    {
        std::cout << "\n";
        if (pickOptionFromList({"Continue", "Customize"}) == 0)
        {
            return attributes;
        }

        while (true)
        {
            clearScreen();

            std::cout << "\n\nOld attributes:\n";
            printAttributes(attributes);

            int pointsSum = STARTING_POINTS;

            attributes.strength = 0;
            attributes.dexterity = 0;
            attributes.vitality = 0;
            attributes.intelligence = 0;

            std::cout << "\nPoints reset to: " << pointsSum << "\n\n";

            if (pointsSum > 0)
            {
                std::cout << "Pick Strength value: ";
                attributes.strength = pickOption(pointsSum);
                pointsSum -= attributes.strength;
            }

            if (pointsSum > 0)
            {
                std::cout << "Pick Dexterity value: ";
                attributes.dexterity = pickOption(pointsSum);
                pointsSum -= attributes.dexterity;
            }

            if (pointsSum > 0)
            {
                std::cout << "Pick Vitality value: ";
                attributes.vitality = pickOption(pointsSum);
                pointsSum -= attributes.vitality;
            }

            if (pointsSum > 0)
            {
                std::cout << "Pick Intelligence value: ";
                attributes.intelligence = pickOption(pointsSum);
                pointsSum -= attributes.intelligence;
            }

            std::cout << "Unspent points: " << pointsSum << "\n";
            attributes.unspentPoints = pointsSum;

            std::cout << "\nNew attributes:\n";
            printAttributes(attributes);

            if (askConfirmation("\n\nDo you accept the new attributes?"))
            {
                break;
            }
        }

        return attributes;
    }

    std::vector<Hero> createHeroes()
    {
        std::vector<Hero> heroes;

        clearScreen();

        // pick number of players
        std::cout << "\nPlease select the number of heroes.\n";
        unsigned int numHeroes = pickOption(4);

        clearScreen();

        // setup the players
        heroes.clear();
        while (heroes.size() < numHeroes)
        {
            std::cout << "\nPlease enter the hero's name\n";
            auto name = enterName();

            if (isNameAlreadyInUse(name, heroes))
            {
                std::cout << "\nName " << name << " is already in use.\nPlease choose a different name\n\n";
                continue;
            }

            clearScreen();

            Hero hero = pickHeroPreset();
            hero.name = name;
            hero.attributes = updateAttributes(hero.attributes);

            Abilities::learnAbility(hero, Abilities::pickStartingAbility());

            auto startingItem = InventoryManager::pickStartingItem(hero.attributes);
            InventoryManager::equipItem(hero, startingItem, EquipmentSlot::MainHand);

            clearScreen();

            if (numHeroes == 1)
            {
                heroes.push_back(hero);
                break;
            }

            printHero(hero);
            if (askConfirmation("\n\nDo you accept?"))
            {
                heroes.push_back(hero);
            }

            clearScreen();
        }

        clearScreen();

        std::cout << "\nList of Heroes:\n";
        for (auto h : heroes)
        {
            std::cout << "\n";
            printHero(h);
        }
        std::cout << "\n";

        return heroes;
    }

}
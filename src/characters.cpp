#include "characters.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>

namespace Characters
{
    Hero pickHeroRace(std::string name)
    {
        auto prompt = []()
        {
            printBorder(130);
            std::cout << "|Please select your race:\n";
            printBorder(130);
            std::cout << "\n";
        };

        std::vector<std::string> menu;

        for (auto rId : startingRaces)
        {
            auto r = g_AllRaces.at(rId);
            menu.push_back(r.name + "\n  " + r.description);
        }

        int selection = pickOptionFromList(prompt, menu);
        auto race = g_AllRaces.at(startingRaces[selection]);
        return Hero{name, STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, STARTING_POINTS, race.id, Controller::Player, race.attributes, {}, {}, basicInventory};
    }

    void assignAttributePoints(Hero &hero)
    {
        int availablePoints = hero.unspentPoints;
        if (availablePoints <= 0)
        {
            return;
        }

        while (true)
        {

            Attributes prevAtt = hero.attributes;
            Attributes attributes = {0, 0, 0, 0};

            clearScreen();

            availablePoints = hero.unspentPoints;

            std::vector<std::pair<std::string, int>> menu{
                {"Strength", prevAtt.strength},
                {"Dexterity", prevAtt.dexterity},
                {"Vitality", prevAtt.vitality},
                {"Intelligence", prevAtt.intelligence},
            };

            auto prompt = [availablePoints, hero]()
            {
                printHeroHeader(hero.name, hero.level);
                std::cout << "\n";
                printBorder(130);
                std::cout << "|Assign points to attributes\n";
                printBorder(130);
                std::cout << "\n\n";
            };

            auto distributedPoints = pointsDistributionMenu(prompt, menu, availablePoints);

            auto results = distributedPoints.first;
            attributes.strength = results[0];
            attributes.dexterity = results[1];
            attributes.vitality = results[2];
            attributes.intelligence = results[3];

            availablePoints = distributedPoints.second;

            clearScreen();

            std::cout << "Unspent points: " << availablePoints << "\n\n";

            printBorder(55);
            std::cout << "|"
                      << "New attributes:\n";
            printBorder(55);

            printAttributesAdjustment(hero.attributes, attributes);
            printBorder(55);

            if (askConfirmation("\n\nDo you accept the new attributes?"))
            {
                hero.unspentPoints = availablePoints;
                hero.attributes = attributes;
                break;
            }
        }
    }

    std::vector<Hero> createHeroes()
    {
        std::vector<Hero> heroes;

        clearScreen();

        // pick number of players
        auto prompt = []()
        {
            printBorder(130);
            std::cout << "|Please select the number of heroes.\n";
            printBorder(130);
            std::cout << "\n";
        };
        unsigned int numHeroes = slider(prompt, 1, 4);

        clearScreen();

        // setup the players
        heroes.clear();
        while (heroes.size() < numHeroes)
        {
            printBorder(130);
            std::cout << "|Please enter the hero's name\n";
            printBorder(130);
            std::cout << "\n";
            auto name = enterName();

            if (isNameAlreadyInUse(name, heroes))
            {
                std::cout << "\nName " << name << " is already in use.\nPlease choose a different name\n\n";
                continue;
            }

            clearScreen();

            Hero hero = pickHeroRace(name);

            assignAttributePoints(hero);

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

        printListOfHeroes(heroes);

        return heroes;
    }
}
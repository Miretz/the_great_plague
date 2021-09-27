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
        { std::cout << "Please select your race\n\n"; };

        std::vector<std::string> menu;

        for (auto r : startingRaces)
        {
            menu.push_back(r.name + " - " + r.description);
        }

        int selection = pickOptionFromList(prompt, menu);
        auto race = startingRaces[selection];
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

            auto prompt = [availablePoints]()
            {
                std::cout << "Assign points to attributes\n\n";
            };

            auto results = pointsDistributionMenu(prompt, menu, availablePoints);

            attributes.strength = results[0];
            attributes.dexterity = results[1];
            attributes.vitality = results[2];
            attributes.intelligence = results[3];

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
        { std::cout << "\nPlease select the number of heroes.\n"; };
        unsigned int numHeroes = slider(prompt, 1, 4);

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
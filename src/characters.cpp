#include "characters.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>

namespace Characters
{
    Hero pickHeroRace(std::string name)
    {
        std::cout << "\nPlease select your race\n";

        for (size_t i = 0; i < startingRaces.size(); i++)
        {
            printBorder(130);
            std::cout << "|" << i + 1 << ".) " << START_GREEN << startingRaces[i].name << END_GREEN << " - " << startingRaces[i].description << "\n";
            printBorder(130);
            printAttributes(startingRaces[i].attributes);
        }
        printBorder(130);

        int selection = pickOption(startingRaces.size());
        auto race = startingRaces[selection - 1];
        return Hero{name, STARTING_HEALTH, 0, STARTING_HEALTH, 1, 100, STARTING_POINTS, race.id, Controller::Player, race.attributes, {}, {}, basicInventory};
    }

    void assignAttributePoints(Hero &hero)
    {
        while (true)
        {

            Attributes attributes = {0, 0, 0, 0};
            int availablePoints = hero.unspentPoints;

            int selected;
            if (availablePoints > 0)
            {
                clearScreen();
                std::cout << "\n\nAssign attribute points:\n";
                printBorder(55);
                printAttributesAdjustment(hero.attributes, attributes);
                printBorder(55);

                std::cout << START_GREEN << "\nPoints remaining: " << availablePoints << END_GREEN << "\n\n";
                std::cout << "Add points to Strength (Current: " << attributes.strength << ")\n";
                selected = pickOptionZeroBased(availablePoints);
                attributes.strength += selected;
                availablePoints -= selected;
            }

            if (availablePoints > 0)
            {
                clearScreen();
                std::cout << "\n\nAssign attribute points:\n";
                printBorder(55);
                printAttributesAdjustment(hero.attributes, attributes);
                printBorder(55);

                std::cout << START_GREEN << "\nPoints remaining: " << availablePoints << END_GREEN << "\n\n";
                std::cout << "Add points to Dexterity: (Current: " << attributes.dexterity << ")\n";
                selected = pickOptionZeroBased(availablePoints);
                attributes.dexterity += selected;
                availablePoints -= selected;
            }

            if (availablePoints > 0)
            {
                clearScreen();
                std::cout << "\n\nAssign attribute points:\n";
                printBorder(55);
                printAttributesAdjustment(hero.attributes, attributes);
                printBorder(55);
                
                std::cout << START_GREEN << "\nPoints remaining: " << availablePoints << END_GREEN << "\n\n";
                std::cout << "Add points to Vitality: (Current: " << attributes.vitality << ")\n";
                selected = pickOptionZeroBased(availablePoints);
                attributes.vitality += selected;
                availablePoints -= selected;
            }

            if (availablePoints > 0)
            {
                clearScreen();
                std::cout << "\n\nAssign attribute points:\n";
                printBorder(55);
                printAttributesAdjustment(hero.attributes, attributes);
                printBorder(55);
                
                std::cout << START_GREEN << "\nPoints remaining: " << availablePoints << END_GREEN << "\n\n";
                std::cout << "Add points to intelligence: (Current: " << attributes.intelligence << ")\n";
                selected = pickOptionZeroBased(availablePoints);
                attributes.intelligence += selected;
                availablePoints -= selected;
            }

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
                hero.attributes = {
                    hero.attributes.strength + attributes.strength,
                    hero.attributes.dexterity + attributes.dexterity,
                    hero.attributes.vitality + attributes.vitality,
                    hero.attributes.intelligence + attributes.intelligence
                };
                break;
            }
        }
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
#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "status_effects.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>

void startJourney(std::vector<Hero> heroes)
{
    // Simulate 3 level ups
    if (!askConfirmation("Simulate 3 level ups & inventory management"))
    {
        exit(0);
    }

    for (int i = 0; i < 3; i++)
    {
        for (auto &h : heroes)
        {
            h.level += 1;
            h.unspentPoints += 1;

            Characters::assignAttributePoints(h);

            InventoryManager::selectEquipment(h);
        }
    }
    exit(0);
}

int main(void)
{

    bool running = true;
    while (running)
    {
        clearScreen();

        auto selection = pickOptionFromList(printIntro, {"Start new journey", "Save", "Load", "Exit"});

        if (selection == 0)
        {
            auto heroes = Characters::createHeroes();
            if (askConfirmation("Begin your journey?"))
            {
                startJourney(heroes);
            }
            else
            {
                continue;
            }
        }
        else if (selection == 1)
        {
            std::cout << "Not yet implmenented!\n\n";
            exit(0);
        }
        else if (selection == 2)
        {
            std::cout << "Not yet implmenented!\n\n";
            exit(0);
        }
        else if (selection == 3)
        {
            std::cout << "Thank you for playing!\n\n";
            exit(0);
        }
    }

    return 0;
}

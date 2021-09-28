#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "status_effects.hpp"
#include "abilities.hpp"
#include "inventory.hpp"
#include "areas.hpp"

#include <iostream>

bool startJourney(std::vector<Hero> heroes)
{
    Areas::navigateToArea(0, heroes);

    return true;
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
                if (startJourney(heroes))
                {
                    continue;
                };
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

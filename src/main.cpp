#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "status_effects.hpp"
#include "abilities.hpp"
#include "inventory.hpp"
#include "areas.hpp"

#include <iostream>

void startJourney(std::vector<Hero> heroes)
{
    Areas::navigateToArea(0, heroes);
}

int main(void)
{

    bool running = true;
    while (running)
    {
        clearScreen();

        auto selection = pickOptionFromList(printIntro, {"Start new journey", "Continue journey", "Exit Game"});

        if (selection == 0)
        {
            if (askConfirmation("Do you want to start a new journey? (Overwrite previous save)"))
            {
                auto heroes = Characters::createHeroes();
                if (askConfirmation("Begin journey?"))
                {
                    startJourney(heroes);
                }
            }
        }
        else if (selection == 1)
        {
            std::cout << "Not yet implmenented!\n\n";
            exit(0);
        }
        else if (selection == 2)
        {
            if (askConfirmation("Are you sure?"))
            {
                clearScreen();
                std::cout << "\n\nThank you for playing!\n\n";
                exit(0);
            }
        }
    }

    return 0;
}

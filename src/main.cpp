#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "status_effects.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>

int main(void)
{

    bool running = true;
    while (running)
    {
        clearScreen();
        printIntro();

        auto selection = pickOptionFromList({"Start new journey", "Save", "Load", "Exit"});
        if (selection == 0)
        {
            auto heroes = Characters::createHeroes();
            if(askConfirmation("Begin your journey?")){
                std::cout << "In journey!\n\n";
                exit(0);
            } else {
                continue;
            }
        }
        else if (selection == 1)
        {
            std::cout << "Not yet implmenented!\n\n";
            exit(1);
        }
        else if (selection == 2)
        {
            std::cout << "Not yet implmenented!\n\n";
            exit(1);
        }
        else if (selection == 3)
        {
            std::cout << "Thank you for playing!\n\n";
            exit(0);
        }
    }

    return 0;
}
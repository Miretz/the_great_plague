#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "abilities.hpp"
#include "inventory.hpp"
#include "areas.hpp"

#include <iostream>

int main(void)
{

    bool running = true;
    while (running)
    {
        clearScreen();

        auto selection = pickOptionFromList(printIntro, {"Start new journey", "Continue journey", "Exit Game"});

        if (selection == 0) // Start new journey
        {
            if (askConfirmation("Do you want to start a new journey? (Overwrite previous save)"))
            {
                auto heroes = Characters::createHeroes();
                if (askConfirmation("Begin journey?"))
                {
                    GameState gs{0, heroes, 1};
                    saveGame(gs);
                    Areas::navigateToArea(gs.areaId, gs);
                }
            }
        }
        else if (selection == 1) // Continue
        {
            auto saveGame = loadGame();
            std::vector<Hero> heroes = saveGame.heroes;
            if (heroes.size() == 0)
            {
                std::cout << "Save file not found!\n\n";
                pressEnterToContinue();
                continue;
            }

            if (askConfirmation("Continue the journey?"))
            {
                clearScreen();
                printListOfHeroes(heroes);
                pressEnterToContinue();
                Areas::navigateToArea(saveGame.areaId, saveGame);
            }
        }
        else if (selection == 2) // Exit
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

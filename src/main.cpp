#include "entities.hpp"
#include "utils.hpp"
#include "characters.hpp"
#include "abilities.hpp"
#include "inventory.hpp"
#include "areas.hpp"
#include "files.hpp"
#include "character_creator.hpp"

#include <iostream>

int main(void)
{

    bool running = true;
    while (running)
    {
        Utils::clearScreen();

        auto selection = Utils::pickOptionFromList(Utils::printIntro, {"Start new journey", "Continue journey", "Exit Game"});

        if (selection == 0) // Start new journey
        {
            if (Utils::askConfirmation("Do you want to start a new journey? (Overwrite previous save)"))
            {
                auto heroes = CharacterCreator::createHeroes();
                if (Utils::askConfirmation("Begin journey?"))
                {
                    GameState gs{0, heroes, 1};
                    Files::saveGame(gs);
                    Areas::navigateToArea(gs.areaId, gs);
                }
            }
        }
        else if (selection == 1) // Continue
        {
            auto saveGame = Files::loadGame();
            std::vector<Hero> heroes = saveGame.heroes;
            if (heroes.size() == 0)
            {
                std::cout << "Save file not found!\n\n";
                Utils::pressEnterToContinue();
                continue;
            }

            if (Utils::askConfirmation("Continue the journey?"))
            {
                Utils::clearScreen();
                Utils::printListOfHeroes(heroes);
                Utils::pressEnterToContinue();
                Areas::navigateToArea(saveGame.areaId, saveGame);
            }
        }
        else if (selection == 2) // Exit
        {
            if (Utils::askConfirmation("Are you sure?"))
            {
                Utils::clearScreen();
                Utils::printBorderedText("Thank you for playing!");
                exit(0);
            }
        }
    }

    return 0;
}

#include "abilities.hpp"
#include "areas.hpp"
#include "character_creator.hpp"
#include "entities.hpp"
#include "files.hpp"
#include "utils.hpp"

int main(void)
{
    Utils::maximizeConsole();
    
    Abilities::init();

    while (true)
    {
        Utils::clearScreen();

        auto selection =
            Utils::pickOptionFromList(Utils::printIntro, { "Start a new journey", "Continue journey", "Exit Game" });

        if (selection == 0)  // Start new journey
        {
            if (Utils::askConfirmation("Do you want to start a new journey? (Overwrite previous save)"))
            {
                auto heroes = CharacterCreator::createHeroes();
                if (Utils::askConfirmation("Begin journey?"))
                {
                    GameState gs{ 0, heroes, {} };
                    Files::saveGame(gs);
                    Areas::navigateToArea(gs.areaId, gs);
                }
            }
        }
        else if (selection == 1)  // Continue
        {
            auto saveGame = Files::loadGame();
            if (saveGame.heroes.size() == 0)
            {
                Utils::printSpacedText("Save file not found!");
                Utils::pressEnterToContinue();
                continue;
            }

            if (Utils::askConfirmation("Continue the journey?"))
            {
                Utils::clearScreen();
                Utils::printListOfHeroes(saveGame.heroes);
                Utils::pressEnterToContinue();
                Areas::navigateToArea(saveGame.areaId, saveGame);
            }
        }
        else if (selection == 2)  // Exit
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

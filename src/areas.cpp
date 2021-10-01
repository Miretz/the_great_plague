#include "areas.hpp"

#include "entities.hpp"
#include "utils.hpp"
#include "files.hpp"
#include "abilities.hpp"
#include "characters.hpp"
#include "inventory_manager.hpp"
#include "combat_system.hpp"

#include <iostream>

namespace Areas
{
    void navigateToArea(uint32_t areaId, GameState &game)
    {
        auto area = allAreas[areaId];

        // change game state
        game.areaId = areaId;

        // print the area description
        Utils::clearScreen();

        // execute auxilary area function
        if (area.auxFunction != nullptr)
        {
            Utils::printBorderedText(area.name);
            area.auxFunction(game);
        }

        // display navigation menu
        std::vector<std::string> menu;
        for (uint32_t aId : area.connections)
        {
            menu.push_back(allAreas[aId].name);
        }
        menu.push_back("Exit");

        // print the area text and navigation menu
        auto areaDescription = Files::loadFile(AREA_FOLDER + area.folder + DESC_FILE);
        auto prompt = [area, areaDescription]()
        {
            Utils::printBorderedText(area.name);
            std::cout << areaDescription;
            std::cout << "Go to:\n";
        };

        auto selection = Utils::pickOptionFromList(prompt, menu);
        if (selection == area.connections.size())
        {
            return;
        }

        navigateToArea(area.connections[selection], game);
    }

    void t01_shore_DanseaConversation(GameState &gameState)
    {
        if (gameState.danseaLocation != 1)
        {
            return;
        }

        for (auto h : gameState.heroes)
        {
            if (h.name == "Dansea")
            {
                return;
            }
        }

        Hero dansea = Files::loadHeroFromConfig(f_danseaConfig);

        auto danseaPicture = Files::loadFile(f_danseaPicture);
        std::cout << danseaPicture;
        std::cout << Files::loadFile(f_danseaIntro);
        Utils::newLine();

        Utils::pressEnterToContinue();
        Utils::clearScreen();

        Utils::pickOptionFromList(
            Utils::createConversationPrompt(dansea.name, "Wait for me!!!", danseaPicture),
            {"(Wait for her to get close)"});

        Utils::pickOptionFromList(
            Utils::createConversationPrompt(dansea.name, "Hey! I bet you're also glad to be out of that boat.", danseaPicture),
            {"Yeah. Where did you get that bow?"});

        Utils::pickOptionFromList(
            Utils::createConversationPrompt(dansea.name, "My father gave it to me so I kept it safe. It reminds me of him.", danseaPicture),
            {"It's always good to keep a weapon on hand. You never know when you need it."});

        uint32_t planQuestion = Utils::pickOptionFromList(
            Utils::createConversationPrompt(dansea.name, "So what's your plan?", danseaPicture),
            {"I am going to the city.", "I don't know yet."});

        if (planQuestion == 0) // I am going to the city.
        {
            if (gameState.heroes.size() == 4) // Party is full
            {
                Utils::clearScreen();
                Utils::createConversationPrompt(dansea.name, "I am going there myself. Maybe we can catch up later. (Dansea leaves the area)", danseaPicture)();
                gameState.danseaLocation = 2;
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
            else // Ask for join
            {

                uint32_t joinQuestion = Utils::pickOptionFromList(
                    Utils::createConversationPrompt(dansea.name, "I was planning to go there. Maybe we can go together?", danseaPicture),
                    {"Sounds good to me.", "No, thanks. I want to explore on my own."});

                if (joinQuestion == 0)
                {
                    gameState.heroes.push_back(dansea);
                    gameState.danseaLocation = 0;
                    Utils::clearScreen();
                    Utils::printBorderedText("Dansea has joined your party.");
                    Utils::printHero(dansea);
                    Utils::newLine();
                    Utils::pressEnterToContinue();
                }
                else
                {
                    Utils::clearScreen();
                    Utils::createConversationPrompt(dansea.name, "Ok, I will head out to the city. (Dansea leaves the area)", danseaPicture)();
                    gameState.danseaLocation = 2;
                    Utils::newLine();
                    Utils::pressEnterToContinue();
                }
            }
        }
        else if (planQuestion == 1) // I don't know yet.
        {
            if (gameState.heroes.size() == 4) // Party is full
            {
                Utils::clearScreen();
                Utils::createConversationPrompt(dansea.name, "I am heading out to the nearest city. Maybe you can find me there. (Dansea leaves the area)", danseaPicture)();
                gameState.danseaLocation = 2;
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
            else
            {

                uint32_t joinQuestion2 = Utils::pickOptionFromList(
                    Utils::createConversationPrompt(dansea.name, "I am heading out to the nearest city. Do you want to join me?", danseaPicture),
                    {"Sounds good to me.", "No. I would like to explore for a bit."});

                if (joinQuestion2 == 0)
                {
                    gameState.heroes.push_back(dansea);
                    gameState.danseaLocation = 0;
                    Utils::clearScreen();
                    Utils::printBorderedText("Dansea has joined your party.");
                    Utils::printHero(dansea);
                    Utils::newLine();
                    Utils::pressEnterToContinue();
                }
                else
                {
                    Utils::clearScreen();
                    Utils::createConversationPrompt(dansea.name, "In that case I'll see you around. (Dansea leaves the area)", danseaPicture)();
                    gameState.danseaLocation = 2;
                    Utils::newLine();
                    Utils::pressEnterToContinue();
                }
            }
        }

        Files::saveGame(gameState);
    }

    void t03_gateCombat(GameState &game)
    {
        auto e1 = Files::loadHeroFromConfig("assets/areas/03_city_gate/enemies/e1.txt");
        auto e2 = Files::loadHeroFromConfig("assets/areas/03_city_gate/enemies/e2.txt");
        CombatSystem::startCombat(game.heroes, {e1, e2});
    }

}
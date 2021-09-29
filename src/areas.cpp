#include "areas.hpp"

#include "utils.hpp"

#include "abilities.hpp"
#include "characters.hpp"
#include "inventory.hpp"

#include <iostream>

namespace Areas
{
    void navigateToArea(uint32_t areaId, GameState &game)
    {
        auto area = allAreas[areaId];

        // change game state
        game.areaId = areaId;

        // print the area description
        clearScreen();

        // execute auxilary area function
        if (area.auxFunction != nullptr)
        {
            printBorder(130);
            std::cout << "|" << area.name << "\n";
            printBorder(130);
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
        auto prompt = [area]()
        {
            printBorder(130);
            std::cout << "|" << area.name << "\n";
            printBorder(130);
            std::cout << area.body;
            std::cout << "Go to:\n\n";
        };

        auto selection = pickOptionFromList(prompt, menu);
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

        std::cout << danseaPicture;
        std::cout << danseaIntro;
        std::cout << "\n\n";

        pressEnterToContinue();
        clearScreen();

        // Create Dansea
        Hero dansea{
            "Dansea",
            100,
            0,
            100,
            1,
            100,
            0,
            Race::Felidae,
            Controller::Player,
            {10, 11, 11, 10},
            {13, 8},
            {},
            {{},
             {
                 {"Torso", 7},
                 {"Legs", 8},
                 {"Head", 9},
                 {"Gloves", 10},
             }}};
        Characters::recalculateHeroHealth(dansea);
        InventoryManager::equipItem(dansea, 5, EquipmentSlot::MainHand);

        pickOptionFromList(
            createConversationPrompt(dansea.name, "Wait for me!!!", danseaPicture),
            {"(Wait for her to get close)"});

        pickOptionFromList(
            createConversationPrompt(dansea.name, "Hey! I bet you're also glad to be out of that boat.", danseaPicture),
            {"Yeah. Where did you get that bow?"});

        pickOptionFromList(
            createConversationPrompt(dansea.name, "My father gave it to me so I kept it safe. It reminds me of him.", danseaPicture),
            {"It's always good to keep a weapon on hand. You never know when you need it."});

        uint32_t planQuestion = pickOptionFromList(
            createConversationPrompt(dansea.name, "So what's your plan?", danseaPicture),
            {"I am going to the city.", "I don't know yet."});

        if (planQuestion == 0) // I am going to the city.
        {
            if (gameState.heroes.size() == 4) // Party is full
            {
                clearScreen();
                createConversationPrompt(dansea.name, "I am going there myself. Maybe we can catch up later. (Dansea leaves the area)", danseaPicture)();
                gameState.danseaLocation = 2;
                pressEnterToContinue();
            }
            else // Ask for join
            {

                uint32_t joinQuestion = pickOptionFromList(
                    createConversationPrompt(dansea.name, "I was planning to go there. Maybe we can go together?", danseaPicture),
                    {"Sounds good to me.", "No, thanks. I want to explore on my own."});

                if (joinQuestion == 0)
                {
                    gameState.heroes.push_back(dansea);
                    gameState.danseaLocation = 0;
                    clearScreen();
                    std::cout << "Dansea has joined your party.\n";
                    printHero(dansea);
                    std::cout << "\n";
                    pressEnterToContinue();
                }
                else
                {
                    clearScreen();
                    createConversationPrompt(dansea.name, "Ok, I will head out to the city. (Dansea leaves the area)", danseaPicture)();
                    gameState.danseaLocation = 2;
                    pressEnterToContinue();
                }
            }
        }
        else if (planQuestion == 1) // I don't know yet.
        {
            if (gameState.heroes.size() == 4) // Party is full
            {
                clearScreen();
                createConversationPrompt(dansea.name, "I am heading out to the nearest city. Maybe you can find me there. (Dansea leaves the area)", danseaPicture)();
                gameState.danseaLocation = 2;
                pressEnterToContinue();
            }
            else
            {

                uint32_t joinQuestion2 = pickOptionFromList(
                    createConversationPrompt(dansea.name, "I am heading out to the nearest city. Do you want to join me?", danseaPicture),
                    {"Sounds good to me.", "No. I would like to explore for a bit."});

                if (joinQuestion2 == 0)
                {
                    gameState.heroes.push_back(dansea);
                    gameState.danseaLocation = 0;
                    clearScreen();
                    std::cout << "Dansea has joined your party.\n";
                    printHero(dansea);
                    std::cout << "\n";
                    pressEnterToContinue();
                }
                else
                {
                    clearScreen();
                    createConversationPrompt(dansea.name, "In that case I'll see you around. (Dansea leaves the area)", danseaPicture)();
                    gameState.danseaLocation = 2;
                    pressEnterToContinue();
                }
            }
        }

        saveGame(gameState);
    }

}
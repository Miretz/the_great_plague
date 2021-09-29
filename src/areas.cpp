#include "areas.hpp"

#include "utils.hpp"

#include "abilities.hpp"

#include <iostream>

namespace Areas
{
    void navigateToArea(uint32_t areaId, std::vector<Hero> &heroes)
    {
        auto area = areas[areaId];

        // print the area description
        clearScreen();

        // execute auxilary area function
        if (area.auxFunction != nullptr)
        {
            printBorder(130);
            std::cout << "|" << area.name << "\n";
            printBorder(130);
            area.auxFunction(heroes);
        }

        // display navigation menu
        std::vector<std::string> menu;
        for (uint32_t aId : area.connections)
        {
            menu.push_back(areas[aId].name);
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

        navigateToArea(area.connections[selection], heroes);
    }

    void t01_shore_DanseaConversation(std::vector<Hero> &heroes)
    {
        std::cout << danseaPicture;
        std::cout << danseaIntro;
        std::cout << "\n\n";

        pressEnterToContinue();
        clearScreen();

        // Create Dansea
        Hero companion_Dansea{
            "Dansea",
            100,
            0,
            100,
            1,
            100,
            0,
            Race::Felidae,
            Controller::Player,
            {2, 12, 5, 5},
            {7},
            {},
            {{},
             {
                 {"Torso", 7},
                 {"Legs", 8},
                 {"Head", 9},
                 {"Gloves", 10},
             }}};

        pickOptionFromList(
            createConversationPrompt(companion_Dansea.name, "Wait for me!!!", danseaPicture),
            {"(Wait for her to get close)"});

        pickOptionFromList(
            createConversationPrompt(companion_Dansea.name, "Hey! I bet you're also glad to be out of that boat.", danseaPicture),
            {"Yeah. Where did you get that bow?"});

        pickOptionFromList(
            createConversationPrompt(companion_Dansea.name, "My father gave it to me so I kept it safe. It reminds me of him.", danseaPicture),
            {"It's always good to keep a weapon on hand. You never know when you need it."});

        uint32_t planQuestion = pickOptionFromList(
            createConversationPrompt(companion_Dansea.name, "So what's your plan?", danseaPicture),
            {"I am going to the city.", "I don't know yet."});

        if (planQuestion == 0)
        {
            if (heroes.size() == 4)
            {
                clearScreen();
                createConversationPrompt(companion_Dansea.name, "I am going there myself. Maybe we can catch up later.", danseaPicture)();
                pressEnterToContinue();
                return;
            }

            uint32_t joinQuestion = pickOptionFromList(
                createConversationPrompt(companion_Dansea.name, "I was planning to go there. Maybe we can go together?", danseaPicture),
                {"Sounds good to me.", "No, thanks. Maybe we can meet later."});

            if (joinQuestion == 0)
            {
                heroes.push_back(companion_Dansea);
                saveGame(heroes, 2);
                clearScreen();
                std::cout << "Dansea has joined your party.\n";
                printHero(companion_Dansea);
                pressEnterToContinue();
            }
            else
            {
                clearScreen();
                createConversationPrompt(companion_Dansea.name, "Ok, see you there.", danseaPicture)();
                pressEnterToContinue();
                return;
            }
        }
        else if (planQuestion == 1)
        {
            uint32_t joinQuestion2 = pickOptionFromList(
                createConversationPrompt(companion_Dansea.name, "I am off to the nearest town. Do you want to join me?", danseaPicture),
                {"Sounds good to me.", "No. I would like to explore for a bit."});

            if (joinQuestion2 == 0)
            {
                heroes.push_back(companion_Dansea);
                saveGame(heroes, 2);
                clearScreen();
                std::cout << "Dansea has joined your party.\n";
                printHero(companion_Dansea);
                pressEnterToContinue();
            }
            else
            {
                clearScreen();
                createConversationPrompt(companion_Dansea.name, "In that case you will probably find me in the nearest town.", danseaPicture)();
                pressEnterToContinue();
                return;
            }
        }
        else
        {
            clearScreen();
            createConversationPrompt(companion_Dansea.name, "Never mind then.", danseaPicture)();
            pressEnterToContinue();
            return;
        }
    }

}
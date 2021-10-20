#include "areas.hpp"

#include "abilities.hpp"
#include "characters.hpp"
#include "combat_system.hpp"
#include "conversation_system.hpp"
#include "entities.hpp"
#include "files.hpp"
#include "inventory_manager.hpp"
#include "utils.hpp"

namespace Areas
{
    void navigateToArea(uint32_t areaId, GameState &game)
    {
        const auto &area = allAreas[areaId];

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
        std::transform(
            area.connections.begin(),
            area.connections.end(),
            std::back_inserter(menu),
            [](uint32_t t) -> std::string
            { return "Go to " + allAreas[t].name; });

        menu.push_back("Open Inventory");
        menu.push_back("Exit");

        // print the area text and navigation menu
        const auto areaDescription = Files::loadFile(AREA_FOLDER + area.folder + DESC_FILE);
        const auto areaImage = Files::loadFile(AREA_FOLDER + area.folder + IMAGE_FILE);
        auto prompt = [&]()
        {
            Utils::printBorderedText(area.name);
            Utils::printArea(areaImage, areaDescription);
        };

        auto selection = Utils::pickOptionFromList(prompt, menu);

        // Exit
        if (selection == menu.size() - 1)
        {
            return;
        }

        // Inventory option
        if (selection == menu.size() - 2)
        {
            InventoryManager::selectPartyEquipment(game.heroes);
            navigateToArea(game.areaId, game);
            return;
        }

        navigateToArea(area.connections[selection], game);
    }

    void t01_shore_DanseaConversation(GameState &gameState)
    {
        if (gameState.stateInfo[propertyDanseaLocation] == 1 || gameState.stateInfo[propertyDanseaLocation] == 2)
        {
            // she is with me or in the city
            return;
        }

        Hero dansea = Files::loadHeroFromConfig(f_danseaConfig);

        const auto danseaPicture = Files::loadFile(f_danseaPicture);
        const auto danseaIntro = Files::loadFile(f_danseaIntro);
        Utils::printArea(danseaPicture, danseaIntro);
        Utils::newLine();

        Utils::pressEnterToContinue();
        Utils::clearScreen();

        const auto conversationResult = ConversationSystem::start(danseaPicture, f_danseaConversation);

        Utils::clearScreen();
        if (conversationResult == ConversationSystem::RESULT_POSITIVE)
        {
            if (gameState.heroes.size() == 4)  // Party is full
            {
                Utils::clearScreen();
                Utils::createConversationPrompt(
                    dansea.name,
                    "Looks like you already have enough people with you. Maybe you can find me later in the city. (Dansea "
                    "leaves the area)",
                    danseaPicture)();
                gameState.stateInfo[propertyDanseaLocation] = 2;
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
            else
            {
                gameState.heroes.push_back(dansea);
                gameState.stateInfo[propertyDanseaLocation] = 1;
                Utils::clearScreen();
                Utils::printBorderedText("Dansea has joined your party.");
                Utils::printHero(dansea);
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
        }
        else
        {
            Utils::clearScreen();
            Utils::createConversationPrompt(
                dansea.name, "In that case, I'll see you around. (Dansea leaves the area)", danseaPicture)();
            gameState.stateInfo[propertyDanseaLocation] = 2;
            Utils::newLine();
            Utils::pressEnterToContinue();
        }

        Files::saveGame(gameState);
    }

    void t03_gateCombat(GameState &gameState)
    {
        const auto property = "gateCombatWon";
        if (gameState.stateInfo[property] == 1)
        {
            return;
        }

        Utils::clearScreen();
        Utils::printCombatStart(Files::loadFile("assets/areas/03_city_gate/combat_intro.txt"));
        Utils::newLine();
        Utils::pressEnterToContinue();

        // generate enemies based on party size
        auto enemyTemplate = Files::loadHeroFromConfig("assets/areas/03_city_gate/enemies/e1.txt");
        auto partySize = gameState.heroes.size();

        // possible names
        std::vector<std::string> names{ "Maurice", "Salomon", "Frederic", "Noel", "Derek", "Daniel", "Calum", "Franz" };
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(names.begin(), names.end(), g);

        // fill the enemies vector
        std::vector<Hero> enemies;
        for (uint32_t i = 0; i < partySize * 2; i++)
        {
            auto h = enemyTemplate;
            h.name = names[i];
            h.uniqueId = h.uniqueId + names[i] + std::to_string(i);
            h.actionPoints = 0;
            enemies.push_back(h);
        }

        auto won = CombatSystem::startCombat(gameState.heroes, enemies);
        if (won)
        {
            Utils::clearScreen();
            Utils::printBorderedText("You have leveled up!");
            Utils::newLine();
            Utils::pressEnterToContinue();

            for (auto &h : gameState.heroes)
            {
                Characters::levelUp(h);
            }
            gameState.stateInfo[property] = 1;
        }
        else
        {
            gameState.stateInfo[property] = 0;
        }
        Files::saveGame(gameState);
    }

    void t04_inkeeperConversation(GameState &gameState)
    {
        const auto areaImage = Files::loadFile(f_innkeeperPicture);
        const auto areaDescription = Files::loadFile(AREA_FOLDER + allAreas[gameState.areaId].folder + DESC_FILE);
        Utils::printArea(areaImage, areaDescription);
        Utils::newLine();

        if (Utils::askConfirmation("Do you want to talk with the Innkeeper?"))
        {
            Utils::clearScreen();
            ConversationSystem::start(areaImage, f_innkeeperConversation);
        }

        Utils::newLine();
        Utils::newLine();
        if (gameState.stateInfo.at(propertyDanseaLocation) == 2 && Utils::askConfirmation("Dansea is also here. Do you want to recruit her?"))
        {
            Hero dansea = Files::loadHeroFromConfig(f_danseaConfig);
            gameState.heroes.push_back(dansea);
            gameState.stateInfo[propertyDanseaLocation] = 1;
            Utils::clearScreen();
            Utils::printBorderedText("Dansea has joined your party.");
            Utils::printHero(dansea);
            Utils::newLine();
            Utils::pressEnterToContinue();
        }

        Files::saveGame(gameState);
    }

}  // namespace Areas
#include "character_creator.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory.hpp"
#include "characters.hpp"

namespace CharacterCreator
{

    std::vector<Hero> createHeroes()
    {
        std::vector<Hero> heroes;

        Utils::clearScreen();

        // pick number of players
        auto prompt = []()
        {
            Utils::printBorderedText("Please select the number of heroes");
        };
        auto numHeroes = Utils::slider(prompt, 1, 4);

        Utils::clearScreen();

        // setup the players
        heroes.clear();
        while (heroes.size() < numHeroes)
        {
            Utils::printBorderedText("Please enter the hero's name");
            auto name = Utils::enterName();

            if (Characters::isNameAlreadyInUse(name, heroes))
            {
                auto warning = "\nThe name " + name + " is already in use. Please choose a different name.";
                Utils::printBorderedText(warning);
                continue;
            }

            Utils::clearScreen();

            auto hero = pickHeroRace(name);

            Characters::assignAttributePoints(hero);

            Abilities::learnAbility(hero, pickStartingAbility());

            auto startingItem = pickStartingItem(hero.attributes);
            InventoryManager::equipItem(hero, startingItem, EquipmentSlot::MainHand);

            Utils::clearScreen();

            if (numHeroes == 1)
            {
                heroes.push_back(hero);
                break;
            }

            Utils::printHero(hero);
            if (Utils::askConfirmation("\n\nDo you accept?"))
            {
                heroes.push_back(hero);
            }

            Utils::clearScreen();
        }

        Utils::clearScreen();

        Utils::printListOfHeroes(heroes);

        return heroes;
    }

    Hero pickHeroRace(std::string name)
    {
        auto prompt = []()
        {
            Utils::printBorderedText("Please select your race:");
        };

        std::vector<std::string> menu;

        for (auto rId : startingRaces)
        {
            auto r = g_AllRaces.at(rId);

            std::string menuOption = r.name + "\n  " + r.description + "\n  " + r.modifierDescription;
            menuOption += "\n  Ability: " + Abilities::allAbilities[r.abilityId].name + " - " + Abilities::allAbilities[r.abilityId].description;
            menu.push_back(menuOption);
        }

        auto selection = Utils::pickOptionFromList(prompt, menu);
        auto race = g_AllRaces.at(startingRaces[selection]);

        auto maxHealth = race.attributes.vitality * Characters::HEALTH_PER_VITALITY_POINT;

        auto hero = Hero{name, maxHealth, 0, maxHealth, 1, 100, STARTING_POINTS, race.id, Controller::Player, race.attributes, {}, {}, basicInventory};
        Abilities::learnAbility(hero, race.abilityId);
        return hero;
    }

    uint32_t pickStartingAbility()
    {
        Utils::clearScreen();

        auto prompt = []()
        {
            Utils::printBorderedText("Pick a starting ability:");
        };

        std::vector<std::string> menu;
        for (auto abilityId : startingAbilities)
        {
            auto sa = Abilities::allAbilities[abilityId];
            menu.push_back(sa.name + " - " + sa.description);
        }

        return startingAbilities[Utils::pickOptionFromList(prompt, menu)];
    }

    uint32_t pickStartingItem(Attributes &attributes)
    {

        std::vector<uint32_t> filtered;
        for (auto itemId : startingItems)
        {
            auto it = g_AllItems[itemId];
            auto reqs = it.requirements;
            if (
                attributes.strength >= reqs.strength &&
                attributes.dexterity >= reqs.dexterity &&
                attributes.vitality >= reqs.vitality &&
                attributes.intelligence >= reqs.intelligence)
            {
                filtered.push_back(itemId);
            }
        }

        Utils::clearScreen();

        auto prompt = []()
        {
            Utils::printBorderedText("Pick a starting item:");
        };

        std::vector<std::string> menu;

        for (auto t : filtered)
        {
            menu.push_back(Utils::getItemString(t));
        }

        return filtered[Utils::pickOptionFromList(prompt, menu)];
    }

}
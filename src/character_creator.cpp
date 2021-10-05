#include "character_creator.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory_manager.hpp"
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
            Characters::assignSpecialtyPoints(hero);

            Abilities::learnAbility(hero, pickStartingAbility());

            auto startingItem = pickStartingItem(hero.attributes);
            InventoryManager::equipItem(hero, startingItem, InventoryManager::EquipmentSlot::MainHand);

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

    Hero pickHeroRace(const std::string &name)
    {
        auto prompt = []()
        {
            Utils::printBorderedText("Please select your race:");
        };

        std::vector<std::string> menu;

        for (auto rId : startingRaces)
        {
            auto r = g_AllRaces.at(rId);

            auto ability = Abilities::getAbility(r.abilityId).value();

            std::string menuOption = r.name;
            menuOption += Utils::COLOR_END;
            menuOption += Utils::COLOR_GREY;
            menuOption += "\n  " + r.description;
            menuOption += "\n  Grants ability: " + ability.name + " - " + ability.description;
            menuOption += Utils::COLOR_END;

            menu.push_back(menuOption);
        }

        auto selection = Utils::pickOptionFromList(prompt, menu);
        auto race = g_AllRaces.at(startingRaces[selection]);

        Attributes defaultAttributes{10, 10, 10, 10};
        Specialties defaultSpecialties{0, 0, 0, 0, 0, 0, 0, 0, 0};
        uint32_t maxHealth = 100;
        uint32_t xp = 0;
        uint32_t level = 1;
        uint32_t xpToLevelUp = 100;

        std::string uniqueId = "PLR_" + name;

        Hero hero{
            uniqueId,
            name,
            maxHealth,
            xp,
            maxHealth,
            level,
            xpToLevelUp,
            STARTING_ATTRIBUTE_POINTS,
            STARTING_SPECIALTY_POINTS,
            startingRaces[selection],
            Controller::Player,
            defaultAttributes,
            defaultSpecialties,
            {race.abilityId},
            basicInventory,
            {}};
        return hero;
    }

    const std::string pickStartingAbility()
    {
        Utils::clearScreen();

        auto prompt = []()
        {
            Utils::printBorderedText("Pick a starting ability:");
        };

        std::vector<std::string> menu;
        for (const auto &abilityId : startingAbilities)
        {
            auto sa = Abilities::getAbility(abilityId).value();
            menu.push_back(sa.name + " - " + sa.description);
        }

        return startingAbilities[Utils::pickOptionFromList(prompt, menu)];
    }

    uint32_t pickStartingItem(Attributes &attributes)
    {

        std::vector<uint32_t> filtered;
        for (auto itemId : startingItems)
        {
            const auto &reqs = g_AllItems[itemId].requirements;
            if (
                attributes.strength >= reqs.strength &&
                attributes.dexterity >= reqs.dexterity &&
                attributes.constitution >= reqs.constitution &&
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
            menu.push_back(Utils::getItemString(t, true));
        }

        return filtered[Utils::pickOptionFromList(prompt, menu)];
    }

}
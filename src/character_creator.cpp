#include "character_creator.hpp"

#include <iterator>

#include "abilities.hpp"
#include "characters.hpp"
#include "inventory_manager.hpp"
#include "utils.hpp"

namespace CharacterCreator
{
    auto createHeroes() -> std::vector<Hero>
    {
        std::vector<Hero> heroes;

        Utils::clearScreen();

        // pick number of players
        auto prompt = []() { Utils::printBorderedText("Please select the number of heroes"); };
        auto numHeroes = Utils::slider(prompt, 1, 4);

        Utils::clearScreen();

        // setup the players
        heroes.clear();
        while (heroes.size() < numHeroes)
        {
            Utils::printBorderedText("Please enter the hero's name");

            const auto name = Utils::enterName();
            if (Characters::isNameAlreadyInUse(name, heroes))
            {
                Utils::printBorderedText("The name " + name + " is already in use. Please choose a different name.");
                continue;
            }

            Utils::clearScreen();

            const auto raceDetail = pickHeroRace();
            auto hero = createHero(name, raceDetail);

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

    auto pickHeroRace() -> std::pair<Race, RaceDetail>
    {
        auto prompt = []() { Utils::printBorderedText("Please select your race:"); };

        std::vector<std::string> menu;

        for (const auto &raceId : startingRaces)
        {
            const auto &race = g_AllRaces.at(raceId);
            const auto defaultRaceAbility = Abilities::getAbility(race.abilityId).value();

            std::string menuOption = race.name;
            menuOption += Utils::kColorEnd;
            menuOption += Utils::kColorGrey;
            menuOption += "\n  " + race.description;
            menuOption += "\n  Grants ability: " + defaultRaceAbility.name + " - " + defaultRaceAbility.description;
            menuOption += Utils::kColorEnd;

            menu.push_back(menuOption);
        }

        auto selection = Utils::pickOptionFromList(prompt, menu);
        return { startingRaces.at(selection), g_AllRaces.at(startingRaces.at(selection)) };
    }

    auto createHero(const std::string &name, const std::pair<Race, RaceDetail> &race) -> Hero
    {
        static constexpr Attributes defaultAttributes{ 10, 10, 10, 10 };
        static constexpr Specialties defaultSpecialties{ 0, 0, 0, 0, 0, 0, 0, 0, 0 };
        static constexpr uint32_t maxHealth = 100;
        static constexpr uint32_t xp = 0;
        static constexpr uint32_t level = 1;
        static constexpr uint32_t xpToLevelUp = 100;

        const std::string uniqueId = "PLR_" + name;

        Hero hero{ uniqueId,
                   name,
                   maxHealth,
                   xp,
                   maxHealth,
                   level,
                   xpToLevelUp,
                   kStartingAttributePoints,
                   kStartingSpecialtyPoints,
                   race.first,
                   Controller::Player,
                   defaultAttributes,
                   defaultSpecialties,
                   { race.second.abilityId },
                   basicInventory,
                   {},
                   0 };
        return hero;
    }

    auto pickStartingAbility() -> std::string
    {
        Utils::clearScreen();

        auto prompt = []() { Utils::printBorderedText("Pick a starting ability:"); };

        std::vector<std::string> menu;
        for (const auto &abilityId : startingAbilities)
        {
            const auto startingAbility = Abilities::getAbility(std::string{ abilityId }).value();
            menu.push_back(Utils::getAbilityString(startingAbility));
        }

        return std::string{ startingAbilities.at(Utils::pickOptionFromList(prompt, menu)) };
    }

    auto pickStartingItem(const Attributes &attributes) -> uint32_t
    {
        std::vector<uint32_t> filtered;
        for (const auto &itemId : startingItems)
        {
            const auto &reqs = g_AllItems[itemId].requirements;
            if (attributes.strength >= reqs.strength && attributes.dexterity >= reqs.dexterity &&
                attributes.constitution >= reqs.constitution && attributes.intelligence >= reqs.intelligence)
            {
                filtered.push_back(itemId);
            }
        }

        Utils::clearScreen();

        auto prompt = []() { Utils::printBorderedText("Pick a starting item:"); };

        std::vector<std::string> menu;
        std::transform(
            filtered.begin(),
            filtered.end(),
            std::back_inserter(menu),
            [](uint32_t t) -> std::string { return Utils::getItemString(t, true); });

        return filtered[Utils::pickOptionFromList(prompt, menu)];
    }

}  // namespace CharacterCreator
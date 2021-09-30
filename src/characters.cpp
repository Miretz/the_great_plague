#include "characters.hpp"

#include "utils.hpp"
#include "abilities.hpp"
#include "inventory.hpp"

#include <iostream>
#include <string>

namespace Characters
{
    

    void assignAttributePoints(Hero &hero)
    {
        auto availablePoints = hero.unspentPoints;
        if (availablePoints <= 0)
        {
            return;
        }

        while (true)
        {

            Attributes prevAtt = hero.attributes;
            Attributes attributes = {0, 0, 0, 0};

            Utils::clearScreen();

            availablePoints = hero.unspentPoints;

            std::vector<std::pair<std::string, uint32_t>> menu{
                {"Strength", prevAtt.strength},
                {"Dexterity", prevAtt.dexterity},
                {"Vitality", prevAtt.vitality},
                {"Intelligence", prevAtt.intelligence},
            };

            auto prompt = [availablePoints, hero]()
            {
                Utils::printHeroHeader(hero.name, hero.level);
                Utils::printBorderedText("Assign points to attributes");
            };

            auto distributedPoints = Utils::pointsDistributionMenu(prompt, menu, availablePoints);

            auto results = distributedPoints.first;
            attributes.strength = results[0];
            attributes.dexterity = results[1];
            attributes.vitality = results[2];
            attributes.intelligence = results[3];

            availablePoints = distributedPoints.second;

            Utils::clearScreen();

            Utils::printBorderedText("Unspent points: " + availablePoints);
            Utils::printBorderedText("New attributes:");

            Utils::printBorder(55);
            Utils::printAttributesAdjustment(hero.attributes, attributes);
            Utils::printBorder(55);

            if (Utils::askConfirmation("\n\nDo you accept the new attributes?"))
            {
                hero.unspentPoints = availablePoints;
                hero.attributes = attributes;
                recalculateHeroHealth(hero);
                break;
            }
        }
    }

    void recalculateHeroHealth(Hero &hero)
    {
        auto newValue = hero.attributes.vitality * HEALTH_PER_VITALITY_POINT * hero.level;
        hero.health = newValue;
        hero.maxHealth = newValue;
    }

    bool isNameAlreadyInUse(std::string name, const std::vector<Hero> &heroes)
    {
        bool sameName = false;
        for (auto h : heroes)
        {
            if (h.name == name)
            {
                sameName = true;
            }
        }
        return sameName;
    }

    
}
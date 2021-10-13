#include "characters.hpp"

#include "abilities.hpp"
#include "entities.hpp"
#include "inventory_manager.hpp"
#include "utils.hpp"

namespace Characters
{
    void levelUp(Hero &hero)
    {
        hero.level += 1;
        hero.unspentAttributePoints += ATTRIBUTE_POINTS_PER_LEVEL;
        hero.unspentSpecialtyPoints += SPECIALTY_POINTS_PER_LEVEL;
        assignAttributePoints(hero);
        assignSpecialtyPoints(hero);
    }

    void assignAttributePoints(Hero &hero)
    {
        auto availablePoints = hero.unspentAttributePoints;
        if (availablePoints == 0)
        {
            return;
        }

        while (true)
        {
            Attributes prevAtt = hero.attributes;
            Attributes attributes = { 0, 0, 0, 0 };

            Utils::clearScreen();

            availablePoints = hero.unspentAttributePoints;

            const PointsMenuInput menu{
                { "Strength",
                  "Strength reflects the raw physical power of a character.\n  "
                  "Grants additional damage with melee weapons such as swords and maces.",
                  prevAtt.strength },
                { "Dexterity",
                  "Dexterity reflects the speed and agility of character's physical movement.\n  "
                  "Grants bonus damage with weapons such as spears, bows and daggers.",
                  prevAtt.dexterity },
                { "Constitution",
                  "Constitution reflects the character's health, toughness, and resilience.\n  "
                  "Grants additional health, armor and various resistances.\n  "
                  "Also increases the proficiency with shields.",
                  prevAtt.constitution },
                { "Intelligence",
                  "Intelligence reflects the character's knowledge and wisdom.\n  "
                  "Grants additional damage with magic weapons and skills.",
                  prevAtt.intelligence },
            };

            auto prompt = [hero]()
            {
                Utils::printHeroHeader(hero.name, hero.level);
                Utils::printBorderedText("Assign points to attributes");
            };

            auto distributedPoints = Utils::pointsDistributionMenu(prompt, menu, availablePoints);

            auto results = distributedPoints.first;
            attributes.strength = results[0];
            attributes.dexterity = results[1];
            attributes.constitution = results[2];
            attributes.intelligence = results[3];

            availablePoints = distributedPoints.second;

            Utils::clearScreen();
            Utils::printBorderedText("Unspent attribute points: " + std::to_string(availablePoints));
            Utils::printBorderedText("New attributes:");

            Utils::printBorder(55);
            Utils::printAttributesAdjustment(hero.attributes, attributes);
            Utils::printBorder(55);

            if (Utils::askConfirmation("\nDo you accept the new attributes?"))
            {
                hero.unspentAttributePoints = availablePoints;
                hero.attributes = attributes;
                recalculateHeroHealth(hero);
                break;
            }
        }
    }

    void assignSpecialtyPoints(Hero &hero)
    {
        auto availablePoints = hero.unspentSpecialtyPoints;
        if (availablePoints == 0)
        {
            return;
        }

        while (true)
        {
            auto previous = hero.specialties;
            Utils::clearScreen();

            availablePoints = hero.unspentSpecialtyPoints;

            const PointsMenuInput menu{
                { "One-handed",
                  "Grants new abilities for one-handed weapons such as daggers, swords and maces.",
                  previous.oneHanded },
                { "Two-handed",
                  "Grants new abilities for two-handed weapons such as longswords and quarterstaffs.",
                  previous.twoHanded },
                { "Ranged",
                  "Grants new abilities for ranged weapons such as bows and crossbows.",
                  previous.ranged },
                { "Dual wielding",
                  "Reduces the penalty of the weapon wielded in off-hand by 5%.",
                  previous.dualWielding },

                { "Mysticism", "Grants abilities that manipulate life force.", previous.mysticism },
                { "Terramancy", "Grants abilities that use the power of nature.", previous.terramancy },
                { "Hydromancy", "Grants abilities that use the power of water and ice.", previous.hydromancy },
                { "Pyromancy", "Grants abilities that use the power of fire.", previous.pyromancy },
                { "Aeromancy", "Grants abilities that use the power of air and electricity.", previous.aeromancy },
            };

            auto prompt = [hero]()
            {
                Utils::printHeroHeader(hero.name, hero.level);
                Utils::printBorderedText("Assign points to specialities");
            };

            auto distributedPoints = Utils::pointsDistributionMenu(prompt, menu, availablePoints);

            auto results = distributedPoints.first;

            Specialties specialties{ results[0], results[1], results[2], results[3], results[4], results[5], results[6], results[7], results[8] };

            availablePoints = distributedPoints.second;

            Utils::clearScreen();
            Utils::printBorderedText("Unspent specialty points: " + std::to_string(availablePoints));
            Utils::printBorderedText("New specialties:");

            Utils::printBorder(55);
            Utils::printSpecialtiesAdjustment(hero.specialties, specialties);
            Utils::printBorder(55);

            if (Utils::askConfirmation("\nDo you accept the new attributes?"))
            {
                hero.unspentSpecialtyPoints = availablePoints;
                hero.specialties = specialties;
                break;
            }
        }
    }

    void recalculateHeroHealth(Hero &hero)
    {
        auto newValue = (hero.attributes.constitution * HEALTH_PER_VITALITY_POINT) + ((hero.level - 1) * HEALTH_PER_LEVEL);
        hero.health = newValue;
        hero.maxHealth = newValue;
    }

    void takeDamage(Hero &hero, const uint32_t amount)
    {
        if (amount >= hero.health)
        {
            hero.health = 0;
        }
        else
        {
            hero.health -= amount;
        }
    }

    void heal(Hero &hero, const uint32_t amount)
    {
        const auto newHealth = hero.health + amount;
        if (newHealth >= hero.maxHealth)
        {
            hero.health = hero.maxHealth;
        }
        else
        {
            hero.health = newHealth;
        }
    }

    bool isNameAlreadyInUse(const std::string &name, const std::vector<Hero> &heroes)
    {
        return std::any_of(heroes.cbegin(), heroes.cend(), [name](const auto &hero)
                           { return hero.name == name; });
    }

}  // namespace Characters
#include "combat_system.hpp"

#include "entities.hpp"
#include "utils.hpp"
#include "abilities.hpp"
#include "characters.hpp"
#include "inventory_manager.hpp"
#include "dice.hpp"

#include <random>
#include <algorithm>
#include <functional>

namespace CombatSystem
{
    Combat prepare(std::vector<Hero> heroes, std::vector<Hero> enemies)
    {
        // shuffle heroes and enemies
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(heroes.begin(), heroes.end(), g);
        std::shuffle(enemies.begin(), enemies.end(), g);

        std::vector<Hero> turnQueue;

        // alternate enemies and heroes in the queue
        const auto iterations = heroes.size() > enemies.size() ? heroes.size() : enemies.size();
        for (uint32_t i = 0; i < iterations; ++i)
        {
            if (i < enemies.size())
            {
                turnQueue.push_back(enemies[i]);
            }
            if (i < heroes.size())
            {
                turnQueue.push_back(heroes[i]);
            }
        }

        return {1, 0, turnQueue, {}};
    }

    bool isAnyFriendlyAlive(Combat &combat)
    {
        auto isFriendly = [](Hero h)
        {
            return h.controller == Controller::Player ||
                   h.controller == Controller::AI_Friendly;
        };

        return std::find_if(combat.turnQueue.begin(), combat.turnQueue.end(), isFriendly) != combat.turnQueue.end();
    }

    bool isAnyEnemyAlive(Combat &combat)
    {

        auto isEnemy = [](Hero h)
        {
            return h.controller == Controller::AI_Enemy;
        };

        return std::find_if(combat.turnQueue.begin(), combat.turnQueue.end(), isEnemy) != combat.turnQueue.end();
    }

    bool isInvisible(Hero &hero)
    {
        for (const auto &se : hero.statusEffects)
        {
            if (se.type == StatusEffectType::Invisibility)
            {
                return true;
            }
        }

        return false;
    }

    void cleanTurnQueue(Combat &combat)
    {
        auto isDead = [](Hero h)
        {
            return h.health <= 0;
        };

        for (const auto &h : combat.turnQueue)
        {
            if (isDead(h))
            {
                combat.dead.push_back(h);
            }
        }

        combat.turnQueue.erase(
            std::remove_if(combat.turnQueue.begin(), combat.turnQueue.end(), isDead),
            combat.turnQueue.end());
    }

    bool startCombat(std::vector<Hero> heroes, std::vector<Hero> enemies)
    {
        auto combat = prepare(heroes, enemies);

        while (isAnyEnemyAlive(combat) && isAnyFriendlyAlive(combat))
        {
            Utils::clearScreen();

            // Print the combat queue
            Utils::printBorderedText("Combat Queue - Turn: " + std::to_string(combat.turn));
            Utils::newLine();
            Utils::printSpacedText("Next:");
            Utils::printCombatHeroHeader(combat.turnQueue[combat.currentHero]);
            Utils::newLine();
            if (combat.currentHero != combat.turnQueue.size() - 1)
            {
                for (uint32_t i = combat.currentHero + 1; i < combat.turnQueue.size(); ++i)
                {
                    Utils::printCombatHeroHeader(combat.turnQueue[i]);
                }
            }
            if (combat.currentHero != 0)
            {
                for (uint32_t i = 0; i < combat.currentHero; ++i)
                {
                    Utils::printCombatHeroHeader(combat.turnQueue[i]);
                }
            }
            Utils::newLine();
            Utils::pressEnterToContinue();

            executeHeroTurn(combat);

            // delete dead people
            cleanTurnQueue(combat);
            clearStatusEffects(combat);

            // if all heros had their turn
            if (combat.currentHero >= combat.turnQueue.size())
            {
                combat.currentHero = 0;
                combat.turn += 1;
            }
        }

        cleanTurnQueue(combat);
        clearAllStatusEffects(combat);

        // print survivors
        Utils::clearScreen();
        Utils::printBorderedText("Combat Over - Turn: " + std::to_string(combat.turn));
        Utils::printSpacedText("Survivors:");
        for (const auto &h : combat.turnQueue)
        {
            Utils::printCombatHeroHeader(h);
        }
        for (const auto &h : combat.dead)
        {
            Utils::printBorderedText(h.name + " has died.");
        }
        Utils::printSpacedText("Result:");

        // end combat
        if (!isAnyEnemyAlive(combat))
        {
            Utils::printBorderedText(Utils::COLOR_GREEN + "VICTORY!" + Utils::COLOR_END);
            Utils::newLine();
            Utils::pressEnterToContinue();
            return true;
        }
        else
        {
            Utils::printBorderedText(Utils::COLOR_RED + "DEFEAT!" + Utils::COLOR_END);
            Utils::newLine();
            Utils::pressEnterToContinue();
            return false;
        }
    }

    void miss(const Hero &hero, const Hero &target)
    {
        Utils::clearScreen();
        Utils::printCombatHeroHeader(hero);
        Utils::printSpacedText("Miss!");
        Utils::printCombatHeroHeader(target);
        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void printDamageNumbers(uint32_t oldHeroHP, uint32_t oldTargetHP, const Hero &hero, const Hero &target, const std::string &description)
    {
        Utils::clearScreen();
        Utils::printCombatHeroHeader(hero);
        Utils::printCombatHeroHeader(target);
        Utils::printSpacedText(hero.name + " used " + Utils::COLOR_YELLOW + description + Utils::COLOR_END + " on " + target.name + ".");

        if (oldHeroHP < hero.health && hero.uniqueId != target.uniqueId)
        {
            Utils::printSpacedText("Restored health of " + hero.name + ": " + std::to_string(hero.health - oldHeroHP));
        }

        if (oldTargetHP < target.health)
        {
            Utils::printSpacedText("Restored health of " + target.name + ": " + std::to_string(target.health - oldTargetHP));
        }
        else if (oldTargetHP != target.health)
        {
            Utils::printSpacedText("Damage to " + target.name + ": " + std::to_string(oldTargetHP - target.health));
        }

        if (target.health == 0)
        {
            Utils::printBorderedText(target.name + " has died.");
        }

        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void basicAttack(Hero &hero, Hero &target)
    {
        auto d20Result = Dice::rollDice(Dice::D20);
        bool critical = d20Result == 20;

        auto damageValue = InventoryManager::getEquippedDamageValue(hero);

        if (critical) // critical always double damage
        {
            damageValue = damageValue * 2;
        }
        else if (damageValue > 5) // non-critical above 5 will get a random small penalty
        {
            damageValue -= Dice::randomSelection(0, 5);
        }

        auto oldHeroHP = hero.health;
        auto oldTargetHP = target.health;

        // check for evading or magic shield
        for (const auto &se : target.statusEffects)
        {
            if (se.type == StatusEffectType::Protection)
            {
                if (Dice::rollDice(Dice::D20) < se.specialValue)
                {
                    printDamageNumbers(oldHeroHP, oldTargetHP, hero, target, "Protected by " + Utils::COLOR_YELLOW + se.name + Utils::COLOR_END);
                    return;
                }
            }
        }

        Characters::takeDamage(target, damageValue);

        auto description = critical ? "Critical Hit" : "Basic Attack";

        printDamageNumbers(oldHeroHP, oldTargetHP, hero, target, description);
    }

    void abilityAttack(Hero &hero, Hero &target, const std::string &abilityId, Combat &combat)
    {
        const auto oldHeroHP = hero.health;
        const auto oldTargetHP = target.health;

        Abilities::executeAbility(abilityId, hero, target, combat);

        const auto abilityName = Abilities::getAbility(abilityId).value().name;

        printDamageNumbers(oldHeroHP, oldTargetHP, hero, target, abilityName);
    }

    std::vector<uint32_t> getTargetableHeroes(Combat &combat, bool isBasicAttack, const std::string &abilityId)
    {
        std::vector<uint32_t> targetable;

        if (isBasicAttack)
        {

            for (uint32_t i = 0; i < combat.turnQueue.size(); ++i)
            {
                if (i == combat.currentHero || combat.turnQueue[i].controller == combat.turnQueue[combat.currentHero].controller)
                {
                    continue;
                }

                if (isInvisible(combat.turnQueue[i]))
                {
                    continue;
                }

                targetable.push_back(i);
            }
        }
        else
        {
            const auto target = Abilities::getAbility(abilityId).value().target;
            if (target == Target::Self)
            {
                targetable.push_back(combat.currentHero);
            }
            else
            {
                auto myController = combat.turnQueue[combat.currentHero].controller;

                for (uint32_t i = 0; i < combat.turnQueue.size(); ++i)
                {
                    if (target == Target::Friendly &&
                        combat.turnQueue[i].controller == myController)
                    {
                        targetable.push_back(i);
                    }
                    if (target == Target::Enemy &&
                        combat.turnQueue[i].controller != myController)
                    {
                        if (i == combat.currentHero) // skip current hero
                        {
                            continue;
                        }

                        if (isInvisible(combat.turnQueue[i]))
                        {
                            continue;
                        }

                        targetable.push_back(i);
                    }
                }
            }
        }

        return targetable;
    }

    // TODO: extract to AI system later
    void executeHeroAITurn(Combat &combat)
    {
        auto &hero = combat.turnQueue[combat.currentHero];

        auto d20Result = Dice::rollDice(Dice::D20);
        const auto isSkipTurn = d20Result == 1;
        if (!isSkipTurn)
        {
            auto selection = Dice::randomSelection(0, hero.abilities.size());
            auto isBasicAttack = selection == hero.abilities.size();

            // get ability id
            std::string abilityId = "";
            if (!isBasicAttack)
            {
                abilityId = hero.abilities[selection];
            }

            // pick enemy
            auto targetable = getTargetableHeroes(combat, isBasicAttack, abilityId);
            if (targetable.empty())
            {
                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Skipped their turn.");
                Utils::newLine();
                Utils::pressEnterToContinue();
                combat.currentHero += 1;
                return;
            }

            auto &target = combat.turnQueue[targetable[Dice::randomSelection(0, targetable.size() - 1)]];

            if (Dice::rollDice(Dice::D20) == 1) // Miss
            {
                miss(hero, target);
            }
            else if (isBasicAttack) // Basic Attack
            {
                basicAttack(hero, target);
            }
            else // Ability
            {
                abilityAttack(hero, target, abilityId, combat);
            }
        }
        else
        {
            Utils::clearScreen();
            Utils::printCombatHeroHeader(hero);
            Utils::printSpacedText("Skipped their turn.");
            Utils::newLine();
            Utils::pressEnterToContinue();
        }

        combat.currentHero += 1;
    }

    void executeHeroTurn(Combat &combat)
    {
        Utils::clearScreen();

        auto &hero = combat.turnQueue[combat.currentHero];

        // check status effects
        for (auto &se : hero.statusEffects)
        {
            se.turnsLeft -= 1;

            if (se.type == StatusEffectType::SkipTurn)
            {
                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Turn skipped because of " + Utils::COLOR_YELLOW + se.name + Utils::COLOR_END);
                Utils::newLine();
                Utils::pressEnterToContinue();

                combat.currentHero += 1;
                return;
            }
            else if (se.type == StatusEffectType::Damage)
            {
                Characters::takeDamage(hero, se.specialValue);

                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Recieved " + std::to_string(se.specialValue) +
                                       " damage from " + Utils::COLOR_YELLOW + se.name + Utils::COLOR_END);
                Utils::newLine();
                Utils::pressEnterToContinue();

                // check for death
                if (hero.health == 0)
                {
                    combat.currentHero += 1;
                    return;
                }
            }
            else if (se.type == StatusEffectType::Heal)
            {
                Characters::heal(hero, se.specialValue);

                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Recieved " + std::to_string(se.specialValue) +
                                       " health from " + Utils::COLOR_YELLOW + se.name + Utils::COLOR_END);
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
        }

        // check if AI turn
        if (hero.controller == Controller::AI_Enemy)
        {
            executeHeroAITurn(combat);
            return;
        }

        //pick action
        auto pickActionPrompt = [hero]()
        {
            Utils::printCombatHeroHeader(hero);
            Utils::printBorderedText("Pick action:");
        };

        std::vector<std::string> actions;
        for (const auto &abilityId : hero.abilities)
        {
            const auto name = Abilities::getAbility(abilityId).value().name;
            actions.push_back("Use " + name);
        }
        actions.push_back("Basic Attack");
        actions.push_back("Skip Turn");

        const auto selection = Utils::pickOptionFromList(pickActionPrompt, actions);
        const auto isSkipTurn = selection == actions.size() - 1;
        const auto isBasicAttack = selection == actions.size() - 2;

        Utils::clearScreen();

        // check if skipping turn
        if (!isSkipTurn)
        {
            // remove invisibility if the hero didn't skip turn
            for (auto &se : hero.statusEffects)
            {
                if (se.type == StatusEffectType::Invisibility)
                {
                    se.turnsLeft = 0;
                }
            }

            // get ability id
            std::string abilityId = "";
            if (!isBasicAttack)
            {
                abilityId = hero.abilities[selection];
            }

            // get targetable heroes
            auto targetable = getTargetableHeroes(combat, isBasicAttack, abilityId);

            // pick target prompt
            auto pickTargetPrompt = [hero]()
            {
                Utils::printCombatHeroHeader(hero);
                Utils::printBorderedText("Pick target:");
            };

            // pick target menu
            std::vector<std::string> targets;
            for (auto t : targetable)
            {
                const auto &h = combat.turnQueue[t];
                const auto color = h.controller == Controller::AI_Enemy ? Utils::COLOR_RED : Utils::COLOR_GREEN;
                targets.push_back("Target " + color + h.name + Utils::COLOR_END + " (Level: " + std::to_string(h.level) +
                                  " HP: " + std::to_string(h.health) + "/" + std::to_string(h.maxHealth) + ")");
            }

            // selected target
            auto &target = combat.turnQueue[targetable[Utils::pickOptionFromList(pickTargetPrompt, targets)]];

            // roll for Miss chance (5%)
            auto d20Result = Dice::rollDice(Dice::D20);
            if (d20Result == 1)
            {
                miss(hero, target);
            }
            else if (isBasicAttack)
            {
                basicAttack(hero, target);
            }
            else
            {
                abilityAttack(hero, target, abilityId, combat);
            }
        }

        combat.currentHero += 1;
    }

    void clearStatusEffects(Combat &combat)
    {
        for (auto &h : combat.turnQueue)
        {
            std::vector<StatusEffect> newEffects;
            for (const auto &se : h.statusEffects)
            {
                if (se.turnsLeft > 0)
                {
                    newEffects.push_back(se);
                }
            }
            h.statusEffects = newEffects;
        }
    }

    void clearAllStatusEffects(Combat &combat)
    {
        for (auto &h : combat.turnQueue)
        {
            h.statusEffects = {};
        }
    }

}

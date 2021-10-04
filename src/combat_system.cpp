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

        const auto q = combat.turnQueue;
        return std::find_if(q.begin(), q.end(), isFriendly) != q.end();
    }

    bool isAnyEnemyAlive(Combat &combat)
    {

        auto isEnemy = [](Hero h)
        {
            return h.controller == Controller::AI_Enemy;
        };

        const auto q = combat.turnQueue;
        return std::find_if(q.begin(), q.end(), isEnemy) != q.end();
    }

    void cleanTurnQueue(Combat &combat)
    {
        auto isDead = [](Hero h)
        {
            return h.health <= 0;
        };

        for (auto h : combat.turnQueue)
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
                Utils::printSpacedText("Waiting for turn:");
                for (uint32_t i = combat.currentHero + 1; i < combat.turnQueue.size(); ++i)
                {
                    Utils::printCombatHeroHeader(combat.turnQueue[i]);
                }
                Utils::newLine();
            }
            if (combat.currentHero != 0)
            {
                Utils::printSpacedText("Waiting for next turn:");
                for (uint32_t i = 0; i < combat.currentHero; ++i)
                {
                    Utils::printCombatHeroHeader(combat.turnQueue[i]);
                }
                Utils::newLine();
            }
            Utils::pressEnterToContinue();

            executeHeroTurn(combat);

            // delete dead people
            cleanTurnQueue(combat);

            // if all heros had their turn
            if (combat.currentHero >= combat.turnQueue.size())
            {
                combat.currentHero = 0;
                combat.turn += 1;
            }
        }

        cleanTurnQueue(combat);

        // print survivors
        Utils::clearScreen();
        Utils::printBorderedText("Combat Over - Turn: " + std::to_string(combat.turn));
        Utils::printSpacedText("Survivors:");
        for (auto h : combat.turnQueue)
        {
            Utils::printCombatHeroHeader(h);
        }
        for (auto h : combat.dead)
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

        if (oldHeroHP < hero.health && hero.name != target.name)
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

        Characters::takeDamage(target, damageValue);

        auto description = critical ? "Critical Hit" : "Basic Attack";

        printDamageNumbers(oldHeroHP, oldTargetHP, hero, target, description);
    }

    void abilityAttack(Hero &hero, Hero &target, const uint32_t abilityId, Combat &combat)
    {
        const auto ability = Abilities::allAbilities[abilityId];

        auto oldHeroHP = hero.health;
        auto oldTargetHP = target.health;

        if (ability.action != nullptr)
        {
            ability.action(hero, target, combat);
        }

        printDamageNumbers(oldHeroHP, oldTargetHP, hero, target, ability.name);
    }

    std::vector<uint32_t> getTargetableHeroes(Combat &combat, bool isBasicAttack, uint32_t abilityId)
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
                targetable.push_back(i);
            }
        }
        else
        {
            auto ability = Abilities::allAbilities[abilityId];

            const auto target = ability.target;

            if (target == Abilities::Target::Self)
            {
                targetable.push_back(combat.currentHero);
            }
            else
            {
                auto myController = combat.turnQueue[combat.currentHero].controller;

                for (uint32_t i = 0; i < combat.turnQueue.size(); ++i)
                {
                    if (target == Abilities::Target::Friendly &&
                        combat.turnQueue[i].controller == myController)
                    {
                        targetable.push_back(i);
                    }
                    if (target == Abilities::Target::Enemy &&
                        combat.turnQueue[i].controller != myController)
                    {
                        if (i == combat.currentHero) // skip current hero
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
            uint32_t abilityId = 0;
            if (!isBasicAttack)
            {
                abilityId = hero.abilities[selection];
            }

            // pick enemy
            auto targetable = getTargetableHeroes(combat, isBasicAttack, abilityId);
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
        for (auto abilityId : hero.abilities)
        {
            auto sa = Abilities::allAbilities[abilityId];
            actions.push_back("Use " + sa.name);
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
            // get ability id
            uint32_t abilityId = 0;
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
                auto h = combat.turnQueue[t];
                const auto color = h.controller == Controller::AI_Enemy ? Utils::COLOR_RED : Utils::COLOR_GREEN;
                targets.push_back("Target " + color + h.name + Utils::COLOR_END + " (Level: " + std::to_string(h.level) + " HP: " + std::to_string(h.health) + "/" + std::to_string(h.maxHealth) + ")");
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

}

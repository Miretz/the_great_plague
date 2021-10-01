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
        std::random_shuffle(heroes.begin(), heroes.end());
        std::random_shuffle(enemies.begin(), enemies.end());

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

        return {1, 0, turnQueue};
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

        combat.turnQueue.erase(
            std::remove_if(combat.turnQueue.begin(), combat.turnQueue.end(), isDead),
            combat.turnQueue.end());
    }

    void startCombat(std::vector<Hero> heroes, std::vector<Hero> enemies)
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
                std::random_shuffle(combat.turnQueue.begin(), combat.turnQueue.end());
                combat.currentHero = 0;
                combat.turn += 1;
            }
        }

        if (!isAnyEnemyAlive(combat))
        {
            Utils::printBorderedText(Utils::COLOR_GREEN + "VICTORY!" + Utils::COLOR_END);
            Utils::pressEnterToContinue();
        }
        else
        {
            Utils::printBorderedText(Utils::COLOR_RED + "DEFEAT!" + Utils::COLOR_END);
            Utils::pressEnterToContinue();
        }
    }

    void executeHeroTurn(Combat &combat)
    {
        Utils::clearScreen();

        auto &hero = combat.turnQueue[combat.currentHero];

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
            // pick target
            auto pickTargetPrompt = [hero]()
            {
                Utils::printCombatHeroHeader(hero);
                Utils::printBorderedText("Pick target:");
            };
            std::vector<std::string> targets;
            for (auto h : combat.turnQueue)
            {
                const auto color = h.controller == Controller::AI_Enemy ? Utils::COLOR_RED : Utils::COLOR_GREEN;
                targets.push_back("Attack " + color + h.name + Utils::COLOR_END);
            }
            auto &target = combat.turnQueue[Utils::pickOptionFromList(pickTargetPrompt, targets)];

            // roll for Miss chance (5%)
            auto d20Result = Dice::rollDiceD20();
            if (d20Result == 1)
            {
                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Miss!");
                Utils::printCombatHeroHeader(target);
                Utils::pressEnterToContinue();
            }
            else
            {
                if (isBasicAttack)
                {
                    bool critical = d20Result == 20;

                    auto damageValue = InventoryManager::getEquippedDamageValue(hero);
                    if (critical)
                    {
                        damageValue = damageValue * 2;
                    }

                    Characters::takeDamage(target, damageValue);

                    Utils::clearScreen();
                    Utils::printCombatHeroHeader(hero);
                    if (critical)
                    {
                        Utils::printSpacedText("Critical hit on: ");
                    }
                    else
                    {
                        Utils::printSpacedText("Used basic attack on: ");
                    }
                    Utils::printCombatHeroHeader(target);
                    Utils::printSpacedText("Damage: " + std::to_string(damageValue));
                    Utils::pressEnterToContinue();
                }
                else
                {
                    const auto ability = Abilities::allAbilities[hero.abilities[selection]];

                    auto oldHeroHP = hero.health;
                    auto oldTargetHP = target.health;

                    if (ability.action != nullptr)
                    {
                        ability.action(hero, target);
                    }

                    Utils::clearScreen();
                    Utils::printCombatHeroHeader(hero);
                    Utils::printSpacedText("Used ability " + ability.name + " on: ");
                    Utils::printCombatHeroHeader(target);

                    if (oldHeroHP < hero.health)
                    {
                        Utils::printSpacedText("Restored health of " + hero.name + ": " + std::to_string(hero.health - oldHeroHP));
                    }

                    if (oldTargetHP < target.health)
                    {
                        Utils::printSpacedText("Restored health of " + target.name + ": " + std::to_string(hero.health - oldHeroHP));
                    }
                    else
                    {
                        Utils::printSpacedText("Damage: " + std::to_string(oldTargetHP - target.health));
                    }

                    Utils::pressEnterToContinue();
                }
            }
        }

        combat.currentHero += 1;
    }

}
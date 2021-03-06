#include "combat_system.hpp"

#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <sstream>

#include "abilities.hpp"
#include "characters.hpp"
#include "dice.hpp"
#include "entities.hpp"
#include "inventory_manager.hpp"
#include "utils.hpp"

namespace CombatSystem
{
    auto prepare(std::vector<Hero> heroes, std::vector<Hero> enemies) -> Combat
    {
        // shuffle heroes and enemies
        Dice::randomize();
        std::shuffle(heroes.begin(), heroes.end(), Dice::getGlobalRandom());
        std::shuffle(enemies.begin(), enemies.end(), Dice::getGlobalRandom());

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

        return { 1, 0, turnQueue, {}, {} };
    }

    void resetActionPoints(Combat &combat)
    {
        for (auto &h : combat.turnQueue)
        {
            h.actionPoints += 3;
            if (h.actionPoints > kMaxActionPoints)
            {
                h.actionPoints = kMaxActionPoints;
            }
        }
    }

    auto isAnyFriendlyAlive(Combat &combat) -> bool
    {
        auto isFriendly = [](const Hero &h)
        { return h.controller == Controller::Player || h.controller == Controller::AI_Friendly; };

        return std::find_if(combat.turnQueue.begin(), combat.turnQueue.end(), isFriendly) != combat.turnQueue.end();
    }

    auto isAnyEnemyAlive(Combat &combat) -> bool
    {
        auto isEnemy = [](const Hero &h) { return h.controller == Controller::AI_Enemy; };

        return std::find_if(combat.turnQueue.begin(), combat.turnQueue.end(), isEnemy) != combat.turnQueue.end();
    }

    auto isInvisible(const Hero &hero) -> bool
    {
        return std::any_of(
            hero.statusEffects.cbegin(),
            hero.statusEffects.cend(),
            [](const auto &se) { return se.type == StatusEffectType::Invisibility; });
    }

    void cleanTurnQueue(Combat &combat)
    {
        auto isDead = [](const Hero &h) { return h.health == 0; };

        for (const auto &h : combat.turnQueue)
        {
            if (isDead(h))
            {
                combat.dead.push_back(h);
            }
        }

        // move creatures to spawn to the turnQueue creatures
        for (auto &&s : combat.spawnQueue)
        {
            combat.turnQueue.push_back(std::move(s));
        }
        combat.spawnQueue.clear();

        combat.turnQueue.erase(
            std::remove_if(combat.turnQueue.begin(), combat.turnQueue.end(), isDead), combat.turnQueue.end());
    }

    auto startCombat(const std::vector<Hero> &heroes, const std::vector<Hero> &enemies) -> bool
    {
        auto combat = prepare(heroes, enemies);

        resetActionPoints(combat);

        while (isAnyEnemyAlive(combat) && isAnyFriendlyAlive(combat))
        {
            Utils::clearScreen();

            printCombatQueue(combat);
            executeHeroTurn(combat);

            // handle flee
            if (combat.turnQueue.size() == 0)
            {
                Utils::printBorderedTextWithColor("You have fled from the fight!", Utils::kColorRed);
                Utils::newLine();
                Utils::pressEnterToContinue();
                return false;
            }

            // delete dead people
            cleanTurnQueue(combat);
            clearStatusEffects(combat);

            // if all heros had their turn
            if (combat.currentHero >= combat.turnQueue.size())
            {
                combat.currentHero = 0;
                combat.turn += 1;
                resetActionPoints(combat);
            }
        }

        cleanTurnQueue(combat);
        clearAllStatusEffects(combat);

        printCasaulties(combat);

        // end combat
        Utils::printSpacedText("Result:");
        if (!isAnyEnemyAlive(combat))
        {
            Utils::printBorderedTextWithColor("VICTORY!", Utils::kColorGreen);
            Utils::newLine();
            Utils::pressEnterToContinue();
            return true;
        }
        else
        {
            Utils::printBorderedTextWithColor("DEFEAT!", Utils::kColorRed);
            Utils::newLine();
            Utils::pressEnterToContinue();
            return false;
        }
    }

    void miss(Hero &hero, const Hero &target)
    {
        decreaseAP(hero, 1);

        Utils::clearScreen();
        Utils::printCombatHeroHeader(hero);
        Utils::printCombatHeroHeader(target);
        Utils::printSpacedText("Miss!");
        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void printCombatQueue(Combat &combat)
    {
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
        Utils::printBorder();
        if (combat.currentHero != 0)
        {
            for (uint32_t i = 0; i < combat.currentHero; ++i)
            {
                Utils::printCombatHeroHeader(combat.turnQueue[i]);
            }
        }
        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void printCasaulties(Combat &combat)
    {
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
    }

    void printStatus(
        uint32_t oldHeroHP,
        uint32_t oldTargetHP,
        const Hero &hero,
        const Hero &target,
        const std::string &description)
    {
        Utils::clearScreen();
        Utils::printCombatHeroHeader(hero);
        Utils::printCombatHeroHeader(target);

        Utils::printCombatStatusMessage(oldHeroHP, oldTargetHP, hero, target, description);

        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void printStatusMultiple(
        uint32_t oldHeroHP,
        std::vector<uint32_t> oldHps,
        const Hero &hero,
        const std::vector<Hero> &targets,
        const std::string &description)
    {
        Utils::clearScreen();
        Utils::printCombatHeroHeader(hero);

        for (const auto &target : targets)
        {
            Utils::printCombatHeroHeader(target);
        }

        for (uint32_t i = 0; i < oldHps.size(); ++i)
        {
            const auto &target = targets[i];
            const auto oldTargetHP = oldHps[i];

            Utils::printCombatStatusMessage(oldHeroHP, oldTargetHP, hero, target, description);
        }

        Utils::newLine();
        Utils::pressEnterToContinue();
    }

    void basicAttack(Hero &hero, Hero &target)
    {
        // base damage from weapon
        auto damageValue = InventoryManager::getEquippedDamageValue(hero);

        // roll for critical, or penalize the base attack
        auto d20Result = Dice::rollDice(Dice::D20);
        const bool critical = d20Result == 20;
        static constexpr auto randomDamageValue = 5;
        if (critical)  // critical always double damage
        {
            damageValue = damageValue * 2;
        }
        else if (damageValue > randomDamageValue)
        {
            damageValue -= Dice::randomSelection(0, randomDamageValue);
        }

        // we need to remember the health before the attack
        auto oldHeroHP = hero.health;
        auto oldTargetHP = target.health;

        decreaseAP(hero, 1);

        // check for evading or magic shield
        for (const auto &se : target.statusEffects)
        {
            if (se.type == StatusEffectType::Protection)
            {
                if (Dice::rollDice(Dice::D20) < se.specialValue)
                {
                    printStatus(
                        oldHeroHP,
                        oldTargetHP,
                        hero,
                        target,
                        "Protected by " + Utils::colorize(se.name, Utils::kColorYellow));
                    return;
                }
            }
        }

        Characters::takeDamage(target, damageValue);

        const std::string description = critical ? "Critical Hit" : "Basic Attack";  // NOLINT

        printStatus(oldHeroHP, oldTargetHP, hero, target, description);
    }

    void abilityAttack(Hero &hero, Hero &target, const std::string &abilityId, Combat &combat)
    {
        const auto ability = Abilities::getAbility(abilityId).value();

        const auto oldHeroHP = hero.health;

        if (ability.type == AbilityType::AoE_Damage || ability.type == AbilityType::AoE_Healing ||
            ability.type == AbilityType::AoE_Status)
        {
            std::vector<uint32_t> oldHps;
            std::vector<Hero> targets;

            auto targetable = getTargetableHeroes(combat, false, abilityId);
            for (const auto tgId : targetable)
            {
                oldHps.push_back(combat.turnQueue[tgId].health);
                Abilities::executeAbility(abilityId, hero, combat.turnQueue[tgId], combat);
                targets.push_back(combat.turnQueue[tgId]);
            }

            printStatusMultiple(oldHeroHP, oldHps, hero, targets, ability.name);
        }
        else
        {
            const auto oldTargetHP = target.health;
            Abilities::executeAbility(abilityId, hero, target, combat);
            printStatus(oldHeroHP, oldTargetHP, hero, target, ability.name);
        }

        decreaseAP(hero, ability.actionPoints);
    }

    void decreaseAP(Hero &hero, uint32_t amount)
    {
        if (hero.actionPoints <= amount)
        {
            hero.actionPoints = 0;
        }
        else
        {
            hero.actionPoints -= amount;
        }
    }

    auto getTargetableHeroes(Combat &combat, bool isBasicAttack, const std::string &abilityId) -> std::vector<uint32_t>
    {
        std::vector<uint32_t> targetable;

        if (isBasicAttack)
        {
            for (uint32_t i = 0; i < combat.turnQueue.size(); ++i)
            {
                if (i == combat.currentHero ||
                    combat.turnQueue[i].controller == combat.turnQueue[combat.currentHero].controller ||
                    isInvisible(combat.turnQueue[i]) || combat.turnQueue[i].health == 0)
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
                    if (combat.turnQueue[i].health == 0)
                    {
                        continue;
                    }

                    if (target == Target::Friendly && combat.turnQueue[i].controller == myController)
                    {
                        targetable.push_back(i);
                    }
                    if (target == Target::Enemy && combat.turnQueue[i].controller != myController)
                    {
                        if (i == combat.currentHero || isInvisible(combat.turnQueue[i]))
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

    void executeHeroAITurn(Combat &combat)
    {
        auto &hero = combat.turnQueue[combat.currentHero];

        auto d20Result = Dice::rollDice(Dice::D20);
        auto isSkipTurn = d20Result == 1;
        while (!isSkipTurn && hero.actionPoints > 0)
        {
            // filter based on action points
            std::vector<std::string> validAbilityIds;
            for (const auto &abilityId : hero.abilities)
            {
                if (Abilities::getAbility(abilityId).value().actionPoints <= hero.actionPoints)
                {
                    validAbilityIds.push_back(abilityId);
                }
            }

            // roll for ability selection, basic attack or skip
            const auto validAbilitiesSize = static_cast<uint32_t>(validAbilityIds.size());
            const auto selection = Dice::randomSelection(0, validAbilitiesSize);
            const auto isBasicAttack = selection == validAbilitiesSize;

            // skip chance
            if (1 == Dice::rollDice(Dice::D20))
            {
                isSkipTurn = true;
                continue;
            }

            // get ability id
            std::string abilityId = "";
            if (!isBasicAttack)
            {
                abilityId = validAbilityIds[selection];
            }

            // pick enemy
            const auto targetable = getTargetableHeroes(combat, isBasicAttack, abilityId);
            if (targetable.empty())
            {
                isSkipTurn = true;
                continue;
            }

            auto &target = combat.turnQueue[targetable[Dice::randomSelection(0, targetable.size() - 1)]];

            // roll for miss chance
            if (Dice::rollDice(Dice::D20) == 1)  // Miss
            {
                miss(hero, target);
            }
            else if (isBasicAttack)  // Basic Attack
            {
                basicAttack(hero, target);
            }
            else  // Ability
            {
                abilityAttack(hero, target, abilityId, combat);
            }
        }

        if (isSkipTurn)
        {
            Utils::clearScreen();
            Utils::printCombatHeroHeader(hero);
            Utils::printSpacedText("Skipped their turn.");
            Utils::newLine();
            Utils::pressEnterToContinue();
        }

        combat.currentHero += 1;
    }

    void fleeCombat(Combat &combat)
    {
        combat.currentHero = 0;
        combat.turnQueue = {};
    }

    void executeHeroTurn(Combat &combat)
    {
        Utils::clearScreen();

        auto &hero = combat.turnQueue[combat.currentHero];

        auto isSkipTurn = false;

        // check status effects
        for (auto &se : hero.statusEffects)
        {
            se.turnsLeft -= 1;

            if (se.type == StatusEffectType::Damage)
            {
                Characters::takeDamage(hero, se.specialValue);

                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText(
                    "Recieved " + std::to_string(se.specialValue) + " damage from " +
                    Utils::colorize(se.name, Utils::kColorYellow));
                Utils::newLine();
                if (hero.health == 0)
                {
                    Utils::printBorderedText(hero.name + " has died.");
                    Utils::newLine();
                    Utils::pressEnterToContinue();
                    combat.currentHero += 1;
                    return;
                }
                Utils::pressEnterToContinue();
            }
            else if (se.type == StatusEffectType::SkipTurn)
            {
                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText("Turn skipped because of " + Utils::colorize(se.name, Utils::kColorYellow));
                Utils::newLine();
                Utils::pressEnterToContinue();

                isSkipTurn = true;
            }
            else if (se.type == StatusEffectType::Heal)
            {
                Characters::heal(hero, se.specialValue);

                Utils::clearScreen();
                Utils::printCombatHeroHeader(hero);
                Utils::printSpacedText(
                    "Recieved " + std::to_string(se.specialValue) + " health from " +
                    Utils::colorize(se.name, Utils::kColorYellow));
                Utils::newLine();
                Utils::pressEnterToContinue();
            }
        }

        // check if AI turn
        if (!isSkipTurn && hero.controller == Controller::AI_Enemy)
        {
            executeHeroAITurn(combat);
            return;
        }

        // check if skipping turn
        while (!isSkipTurn && hero.actionPoints > 0)
        {
            Utils::clearScreen();

            // pick action
            auto pickActionPrompt = [hero]()
            {
                Utils::printCombatHeroHeader(hero);
                Utils::printBorderedText("Pick action:");
            };

            // filter for valid abilities
            std::vector<std::string> actions;
            std::vector<std::string> validAbilityIds;
            for (const auto &abilityId : hero.abilities)
            {
                const auto ability = Abilities::getAbility(abilityId).value();
                if (ability.actionPoints <= hero.actionPoints)
                {
                    actions.push_back("Use " + ability.name + " (AP: " + std::to_string(ability.actionPoints) + ")");
                    validAbilityIds.push_back(abilityId);
                }
            }
            actions.emplace_back("Basic Attack (AP: 1)");
            actions.emplace_back("Skip Turn");
            actions.emplace_back("[Party] Flee from the fight");

            const auto selection = Utils::pickOptionFromList(pickActionPrompt, actions);

            // handle fleeing from the fight
            if (selection == actions.size() - 1)
            {
                if (Utils::askConfirmation("Are you sure?"))
                {
                    Utils::newLine();
                    Utils::newLine();
                    fleeCombat(combat);
                }
                return;
            }

            isSkipTurn = selection == actions.size() - 2;
            const auto isBasicAttack = selection == actions.size() - 3;

            Utils::clearScreen();

            if (isSkipTurn)
            {
                continue;
            }

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
                abilityId = validAbilityIds[selection];
            }

            // get targetable heroes
            auto targetable = getTargetableHeroes(combat, isBasicAttack, abilityId);

            // no targets
            if (targetable.empty())
            {
                Utils::printBorderedText("No targets in combat area.");
                Utils::newLine();
                Utils::pressEnterToContinue();
                isSkipTurn = true;
                continue;
            }

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
                const auto color = h.controller == Controller::AI_Enemy ? Utils::kColorRed : Utils::kColorGreen;
                targets.push_back(
                    "Target " + Utils::colorize(h.name, color) + " (Level: " + std::to_string(h.level) +
                    " HP: " + std::to_string(h.health) + "/" + std::to_string(h.maxHealth) + ")");
            }

            // selected target
            auto &target = combat.turnQueue[targetable[Utils::pickOptionFromList(pickTargetPrompt, targets)]];

            if (isBasicAttack)
            {
                // roll for Miss chance (5%)
                if (1 == Dice::rollDice(Dice::D20))
                {
                    miss(hero, target);
                }
                else
                {
                    basicAttack(hero, target);
                }
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
            std::copy_if(
                h.statusEffects.begin(),
                h.statusEffects.end(),
                std::back_inserter(newEffects),
                [](const auto &se) { return se.turnsLeft > 0; });
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

}  // namespace CombatSystem

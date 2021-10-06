#pragma once
#ifndef combat_system_hpp
#define combat_system_hpp

#include <string>
#include <vector>
#include <queue>

struct Hero;
struct Combat;

namespace CombatSystem
{
    Combat prepare(std::vector<Hero> heroes, std::vector<Hero> enemies);

    void resetActionPoints(Combat &combat);

    bool isAnyFriendlyAlive(Combat &combat);

    bool isAnyEnemyAlive(Combat &combat);

    bool isInvisible(const Hero &hero);

    void cleanTurnQueue(Combat &combat);

    bool startCombat(std::vector<Hero> heroes, std::vector<Hero> enemies);

    void miss(Hero &hero, const Hero &target);

    void basicAttack(Hero &hero, Hero &target);

    void abilityAttack(Hero &hero, Hero &target, const std::string &abilityId, Combat &combat);

    void printDamageNumbers(uint32_t oldHeroHP, uint32_t oldTargetHP, const Hero &hero, const Hero &target, const std::string &description);

    void printDamageNumbersMultiple(uint32_t oldHeroHP, std::vector<uint32_t> oldHps, const Hero &hero, const std::vector<Hero> &targets, const std::string &description);

    std::vector<uint32_t> getTargetableHeroes(Combat &combat, bool isBasicAttack, const std::string &abilityId);

    void executeHeroAITurn(Combat &combat);

    void executeHeroTurn(Combat &combat);

    void clearStatusEffects(Combat &combat);

    void clearAllStatusEffects(Combat &combat);

    void decreaseAP(Hero &hero, uint32_t amount);

    void fleeCombat(Combat &combat);
}

#endif
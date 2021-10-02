#pragma once
#ifndef combat_system_hpp
#define combat_system_hpp

#include <string>
#include <vector>
#include <queue>

struct Hero;

namespace CombatSystem
{
    struct Combat
    {
        uint32_t turn;
        uint32_t currentHero;
        std::vector<Hero> turnQueue;
    };

    Combat prepare(std::vector<Hero> heroes, std::vector<Hero> enemies);

    bool isAnyFriendlyAlive(Combat &combat);

    bool isAnyEnemyAlive(Combat &combat);

    void cleanTurnQueue(Combat &combat);

    bool startCombat(std::vector<Hero> heroes, std::vector<Hero> enemies);

    void miss(const Hero &hero, const Hero &target);

    void basicAttack(Hero &hero, Hero &target);

    void abilityAttack(Hero &hero, Hero &target, const uint32_t abilityId);

    std::vector<uint32_t> getTargetableHeroes(Combat &combat);

    void executeHeroAITurn(Combat &combat);

    void executeHeroTurn(Combat &combat);

}

#endif
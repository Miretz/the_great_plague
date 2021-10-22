#pragma once
#ifndef combat_system_hpp
#define combat_system_hpp

#include <queue>
#include <string>
#include <vector>

struct Hero;
struct Combat;

namespace CombatSystem
{

    static constexpr auto kMaxActionPoints = 6;

    auto prepare(std::vector<Hero> heroes, std::vector<Hero> enemies) -> Combat;

    void resetActionPoints(Combat &combat);

    auto isAnyFriendlyAlive(Combat &combat) -> bool;

    auto isAnyEnemyAlive(Combat &combat) -> bool;

    auto isInvisible(const Hero &hero) -> bool;

    void cleanTurnQueue(Combat &combat);

    auto startCombat(const std::vector<Hero> &heroes, const std::vector<Hero> &enemies) -> bool;

    void miss(Hero &hero, const Hero &target);

    void basicAttack(Hero &hero, Hero &target);

    void abilityAttack(Hero &hero, Hero &target, const std::string &abilityId, Combat &combat);

    void printDamageNumbers(
        uint32_t oldHeroHP,
        uint32_t oldTargetHP,
        const Hero &hero,
        const Hero &target,
        const std::string &description);

    void printDamageNumbersMultiple(
        uint32_t oldHeroHP,
        std::vector<uint32_t> oldHps,
        const Hero &hero,
        const std::vector<Hero> &targets,
        const std::string &description);

    auto getTargetableHeroes(Combat &combat, bool isBasicAttack, const std::string &abilityId) -> std::vector<uint32_t>;

    void executeHeroAITurn(Combat &combat);

    void executeHeroTurn(Combat &combat);

    void clearStatusEffects(Combat &combat);

    void clearAllStatusEffects(Combat &combat);

    void decreaseAP(Hero &hero, uint32_t amount);

    void fleeCombat(Combat &combat);
}  // namespace CombatSystem

#endif
#ifndef combat_system_hpp
#define combat_system_hpp

#include <queue>
#include <string>
#include <vector>

struct Hero;
struct Combat;

namespace CombatSystem
{
    inline constexpr auto kMaxActionPoints = 6;

    [[nodiscard]] auto prepare(std::vector<Hero> heroes, std::vector<Hero> enemies) -> Combat;

    void resetActionPoints(Combat &combat);

    [[nodiscard]] auto isAnyFriendlyAlive(Combat &combat) -> bool;

    [[nodiscard]] auto isAnyEnemyAlive(Combat &combat) -> bool;

    [[nodiscard]] auto isInvisible(const Hero &hero) -> bool;

    void cleanTurnQueue(Combat &combat);

    [[nodiscard]] auto startCombat(const std::vector<Hero> &heroes, const std::vector<Hero> &enemies) -> bool;

    void miss(Hero &hero, const Hero &target);

    void basicAttack(Hero &hero, Hero &target);

    void abilityAttack(Hero &hero, Hero &target, std::string_view abilityId, Combat &combat);

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

    [[nodiscard]] auto getTargetableHeroes(Combat &combat, bool isBasicAttack, std::string_view abilityId)
        -> std::vector<uint32_t>;

    void executeHeroAITurn(Combat &combat);

    void executeHeroTurn(Combat &combat);

    void clearStatusEffects(Combat &combat);

    void clearAllStatusEffects(Combat &combat);

    void decreaseAP(Hero &hero, uint32_t amount);

    void fleeCombat(Combat &combat);
}  // namespace CombatSystem

#endif
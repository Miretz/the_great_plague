#pragma once

#include "entities.hpp"

namespace StatusEffects
{
    void se_Error(Hero &hero);

    void se_Evasion(Hero &hero);
    void se_Knockdown(Hero &hero);
    void se_MagicShield(Hero &hero);

    const StatusEffect Evasion{ "Evasion", 3, se_Evasion };
    const StatusEffect Knockdown{ "Knockdown", 1, se_Knockdown };
    const StatusEffect MagicShield{ "Magic Shield", 4, se_MagicShield };
}
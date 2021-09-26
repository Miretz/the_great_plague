#include "status_effects.hpp"

#include <iostream>

namespace StatusEffects
{

    void se_Error(Hero &hero)
    {
        // TODO: Implement what happens?
        std::cout << "\nNon-existing status effect called on " << hero.name << "\n";
        exit(-1);
    }

    void se_Evasion(Hero &hero)
    {
        // TODO: Implement what happens?
        std::cout << "\nMissing Implementation of Evasion on " << hero.name << "\n";
        exit(-1);
    }

    void se_Knockdown(Hero &hero)
    {
        // TODO: Implement what happens?
        std::cout << "\nMissing Implementation of Knockdown on " << hero.name << "\n";
        exit(-1);
    }

    void se_MagicShield(Hero &hero)
    {
        // TODO: Implement what happens?
        std::cout << "\nMissing Implementation of Magic Shield on " << hero.name << "\n";
        exit(-1);
    }

}
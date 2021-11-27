#include "files.hpp"

#include "entities.hpp"
#include <gtest/gtest.h>

constexpr auto kHeroSerialized =
    "PLR_testman|testman|100|0|100|1|100|2|2|0|0|10|11|10|10|0|0|0|0|4|0|1|2|0|race_human;first_aid;||Torso;7;|";

TEST(Files, deserializeHero)
{
    const auto hero = Files::deserializeHero(kHeroSerialized);

    EXPECT_STREQ("PLR_testman", hero.uniqueId.c_str());
    EXPECT_STREQ("testman", hero.name.c_str());
    EXPECT_EQ(100u, hero.health);
    EXPECT_EQ(0u, hero.xp);
    EXPECT_EQ(100u, hero.maxHealth);
    EXPECT_EQ(1u, hero.level);
    EXPECT_EQ(100u, hero.xpToLevelUp);
    EXPECT_EQ(Race::Human, hero.race);
    EXPECT_EQ(Controller::Player, hero.controller);
    EXPECT_EQ(1u, hero.inventory.equipped.size());

    EXPECT_STREQ("race_human", hero.abilities[0].c_str());
    EXPECT_STREQ("first_aid", hero.abilities[1].c_str());
}

TEST(Files, serializeHero)
{
    Hero hero;
    hero.uniqueId = "PLR_testman";
    hero.name = "testman";
    hero.health = 100;
    hero.maxHealth = 100;
    hero.xp = 0;
    hero.level = 1;
    hero.xpToLevelUp = 100;
    hero.unspentAttributePoints = 2;
    hero.unspentSpecialtyPoints = 2;
    hero.race = Race::Human;
    hero.controller = Controller::Player;
    hero.attributes = { 10, 11, 10, 10 };
    hero.specialties = { 0, 0, 0, 0, 4, 0, 1, 2, 0 };
    hero.inventory = { {}, { { "Torso", 7 } } };
    hero.abilities = { "race_human", "first_aid" };

    const auto serialized = Files::serializeHero(hero);

    EXPECT_STREQ(kHeroSerialized, serialized.c_str());
}

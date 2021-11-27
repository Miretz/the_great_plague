#include "utils.hpp"

#include <gtest/gtest.h>

TEST(Utils, trim)
{
    EXPECT_STREQ("hello", Utils::trim("  hello   ").c_str());
}

TEST(Utils, addPlus_valid)
{
    EXPECT_STREQ("+4", Utils::addPlus(4).c_str());
}

TEST(Utils, addPlus_zero)
{
    EXPECT_STREQ(" 0", Utils::addPlus(0).c_str());
}
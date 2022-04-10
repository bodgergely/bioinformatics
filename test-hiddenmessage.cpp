#include <gtest/gtest.h>

#include "hiddenmessage.h"

using namespace hiddenmessage;

TEST(StringsEqual, equal)
{
    EXPECT_EQ(StringsEqual("Hello", 0, "Hello", 0, 5), true);
    EXPECT_EQ(StringsEqual("Hello", 3, "lo", 0, 2), true);
}

TEST(StringsEqual, outofbounds)
{
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 0, 3), "");
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 2, 3), "");
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 2, 0), "");
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
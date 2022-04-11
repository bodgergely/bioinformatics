#include <gtest/gtest.h>

#include "hiddenmessage.h"

using namespace hiddenmessage;

TEST(StringsEqual, Equal)
{
    EXPECT_EQ(StringsEqual("Hello", 0, "Hello", 0, 5), true);
    EXPECT_EQ(StringsEqual("Hello", 3, "lo", 0, 2), true);
}

TEST(StringsEqual, OutOfBounds)
{
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 0, 3), "");
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 2, 3), "");
    ASSERT_DEATH(StringsEqual("Hello", 0, "me", 2, 0), "");
}

TEST(HammingDistance, Simple)
{
    EXPECT_EQ(HammingDistance("Hello", "Hello"), 0);
    EXPECT_EQ(HammingDistance("Hello", "Cello"), 1);
}

TEST(HammingDistance, DifferentLength)
{
    EXPECT_EQ(HammingDistance("Hello", "He"), 3);
    string s1{"Hello"};
    EXPECT_EQ(HammingDistance(string_view(s1), string_view(s1.c_str() + 3, 2)),
              5);
    EXPECT_EQ(HammingDistance("Ger", "Gerike"), 3);
}

TEST(PatternCount, Simple)
{
    EXPECT_EQ(PatternCount("Hello", "Hel"), 1);
    EXPECT_EQ(PatternCount("almaalma", "alma"), 2);
}

TEST(PatternCount, Complex)
{
    EXPECT_EQ(PatternCount("aaaaaa", "a"), 6);
    EXPECT_EQ(PatternCount("aaaaaa", "aa"), 5);
    EXPECT_EQ(PatternCount("abcabcabc", "abc"), 3);
    EXPECT_EQ(PatternCount("aaaab", "aa"), 3);
}

TEST(PatternCount, PatternLonger)
{
    EXPECT_EQ(PatternCount("Hell", "Hello"), 0);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
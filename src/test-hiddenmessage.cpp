#include <gtest/gtest.h>

#include <unordered_set>

#include "hiddenmessage.h"

TEST(stringsEqual, Equal)
{
    EXPECT_EQ(stringsEqual("Hello", 0, "Hello", 0, 5), true);
    EXPECT_EQ(stringsEqual("Hello", 3, "lo", 0, 2), true);
}

TEST(stringsEqual, OutOfBounds)
{
    ASSERT_DEATH(stringsEqual("Hello", 0, "me", 0, 3), "");
    ASSERT_DEATH(stringsEqual("Hello", 0, "me", 2, 3), "");
    ASSERT_DEATH(stringsEqual("Hello", 0, "me", 2, 0), "");
}

TEST(hammingDistance, Simple)
{
    EXPECT_EQ(hammingDistance("Hello", "Hello"), 0);
    EXPECT_EQ(hammingDistance("Hello", "Cello"), 1);
}

TEST(hammingDistance, DifferentLength)
{
    EXPECT_EQ(hammingDistance("Hello", "He"), 3);
    string s1{"Hello"};
    EXPECT_EQ(hammingDistance(string_view(s1), string_view(s1.c_str() + 3, 2)),
              5);
    EXPECT_EQ(hammingDistance("Ger", "Gerike"), 3);
}

TEST(patternCount, Simple)
{
    EXPECT_EQ(patternCount("Hello", "Hel"), 1);
    EXPECT_EQ(patternCount("almaalma", "alma"), 2);
}

TEST(patternCount, Complex)
{
    EXPECT_EQ(patternCount("aaaaaa", "a"), 6);
    EXPECT_EQ(patternCount("aaaaaa", "aa"), 5);
    EXPECT_EQ(patternCount("abcabcabc", "abc"), 3);
    EXPECT_EQ(patternCount("aaaab", "aa"), 3);
}

TEST(patternCount, PatternLonger)
{
    EXPECT_EQ(patternCount("Hell", "Hello"), 0);
}

TEST(patternCountApprox, Simple)
{
    string text = "HelloHello";
    EXPECT_EQ(patternCountApprox(text, "Hello", 0),
              patternCount(text, "Hello"));
    EXPECT_EQ(patternCountApprox(text, "Hellb", 0), 0);
    EXPECT_EQ(patternCountApprox(text, "Hellb", 1), 2);
    EXPECT_EQ(patternCountApprox(text, "aaaaa", 5), 6);
    EXPECT_EQ(patternCountApprox(text, "aaaaa", 4), 0);
}

TEST(FrequentWords, ExactMatches)
{
    auto most_freq = FrequentWords::findMostFrequentKmers("bbaaaaacccbaaa", 3);
    EXPECT_EQ(most_freq.first, 4);
    EXPECT_EQ(most_freq.second, vector<string>{"aaa"});
    most_freq = FrequentWords::findMostFrequentKmers("aaaabbbbccccddd", 3);
    EXPECT_EQ(most_freq.first, 2);
    EXPECT_EQ(
        unordered_set<string>(most_freq.second.begin(), most_freq.second.end()),
        unordered_set<string>({"aaa", "bbb", "ccc"}));
    most_freq = FrequentWords::findMostFrequentKmers("aaaabbbbccccddddd", 3);
    EXPECT_EQ(most_freq.first, 3);
    EXPECT_EQ(
        unordered_set<string>(most_freq.second.begin(), most_freq.second.end()),
        unordered_set<string>({"ddd"}));
    most_freq = FrequentWords::findMostFrequentKmers("aaaabbbbccccdddeeeee", 3);
    EXPECT_EQ(most_freq.first, 3);
    EXPECT_EQ(
        unordered_set<string>(most_freq.second.begin(), most_freq.second.end()),
        unordered_set<string>({"eee"}));
    most_freq = FrequentWords::findMostFrequentKmers("aaaaa", 6);
    EXPECT_EQ(most_freq.first, 0);
    EXPECT_EQ(most_freq.second, vector<string>{});
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
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

void testLocations(string_view pattern, string_view text,
                   const unordered_set<int>& expected)
{
    auto locs = findLocationsOfPattern(pattern, text);
    ASSERT_EQ(unordered_set<int>(locs.begin(), locs.end()), expected);
}

TEST(FindLocations, test1)
{
    string text{"GATATATGCATATACTT"};
    string pattern("ATAT");
    unordered_set<int> expected{1, 3, 9};
    testLocations(pattern, text, expected);
}
TEST(FindLocations, test2)
{
    string text{"TTTTACACTTTTTTGTGTAAAAA"};
    string pattern("ACAC");
    unordered_set<int> expected{4};
    testLocations(pattern, text, expected);
}
TEST(FindLocations, test3)
{
    string text{
        "AAAGAGTGTCTGATAGCAGCTTCTGAACTGGTTACCTGCCGTGAGTAAATTAAATTTTATTGACTTAGGT"
        "CACTAAATACTTTAACCAATATAGGCATAGCGCACAGACAGATAATAATTACAGAGTACACAACATCCA"
        "T"};
    string pattern("AAA");
    unordered_set<int> expected{0, 46, 51, 74};
    testLocations(pattern, text, expected);
}
TEST(FindLocations, test4)
{
    string text{
        "AGCGTGCCGAAATATGCCGCCAGACCTGCTGCGGTGGCCTCGCCGACTTCACGGATGCCAAGTGCATAGA"
        "GGAAGCGAGCAAAGGTGGTTTCTTTCGCTTTATCCAGCGCGTTAACCACGTTCTGTGCCGACTTT"};
    string pattern("TTT");
    unordered_set<int> expected{88, 92, 98, 132};
    testLocations(pattern, text, expected);
}
TEST(FindLocations, test5)
{
    string text{"ATATATA"};
    string pattern("ATA");
    unordered_set<int> expected{0, 2, 4};
    testLocations(pattern, text, expected);
}

TEST(ClumpFinding, test1)
{
    // string text{
    //     "CCACGCGGTGTACGCTGCAAAAAGCCTTGCTGAATCAAATAAGGTTCCAGCACATCCTCAATGGTTTCAC"
    //     "GTTCTTCGCCAATGGCTGCCGCCAGGTTATCCAGACCTACAGGTCCACCAAAGAACTTATCGATTACCGC"
    //     "CAGCAACAATTTGCGGTCCATATAATCGAAACCTTCAGCATCGACATTCAACATATCCAGCG"};
    // auto freqs = findClumps(text, 3, 25, 3);
    // EXPECT_EQ(unordered_set<string>(freqs.begin(), freqs.end()),
    //           unordered_set<string>({"AAA", "CAG", "CAT", "GCC", "TTC"}));
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
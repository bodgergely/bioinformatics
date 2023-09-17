#pragma once

#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <utility>

#include "utils.h"

using namespace std;
using ull = unsigned long long;

bool stringsEqual(string_view str1, int idx1, string_view str2, int idx2, int n)
{
    assert((idx1 + n <= str1.length()) && (idx2 + n <= str2.length()));
    assert(n > 0);
    for (int i = 0; i < n; i++) {
        if (str1[idx1 + i] != str2[idx2 + i])
            return false;
    }
    return true;
}

int hammingDistance(string_view s1, string_view s2)
{
    int mlen = s1.length() <= s2.length() ? s1.length() : s2.length();
    int d = 0;
    for (int i = 0; i < mlen; i++) {
        if (s1[i] != s2[i])
            d++;
    }
    d += abs(static_cast<long long>(s1.length() - s2.length()));
    return d;
}

int patternCount(string_view text, string_view pattern)
{
    int count = 0;
    if (pattern.size() > text.size())
        return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if (stringsEqual(text, i, pattern, 0, pattern.size()))
            count++;
    }
    return count;
}

int patternCountApprox(string_view text, string_view pattern, int d)
{
    int count = 0;
    if (pattern.size() > text.size())
        return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if (hammingDistance(string_view(text.begin() + i, pattern.size()),
                            pattern) <= d)
            count++;
    }
    return count;
}

class FrequentWords
{
public:
    // static pair<int, vector<string>> findMostFrequentKmersWithMismatches(
    //     string_view text, int k, int d)
    // {
    //     unordered_map<string, int> freqMap;
    //     for (int i = 0; i < text.length() - k; i++) {
    //         auto pattern = text.substr(i, k);
    //     }
    // }

    static pair<int, vector<string>> findMostFrequentKmers(string_view text,
                                                           int k)
    {
        vector<string> res;
        unordered_map<string, int> frequencyTable =
            buildFrequencyTable(text, k);
        int maxOccurence = maxOccurenceCount(frequencyTable);
        for (const auto& p : frequencyTable) {
            if (p.second == maxOccurence) {
                res.push_back(p.first);
            }
        }
        return make_pair(maxOccurence, res);
    }
    static unordered_map<string, int> buildFrequencyTable(string_view text,
                                                          int k)
    {
        unordered_map<string, int> table;
        if (text.size() < k)
            return table;
        for (size_t i = 0; i <= text.size() - k; i++) {
            table[string(text.begin() + i, text.begin() + i + k)]++;
        }
        return table;
    }

private:
    static int maxOccurenceCount(
        const unordered_map<string, int>& frequencyTable)
    {
        int maxCount = 0;
        for (auto& p : frequencyTable) {
            maxCount = max(maxCount, p.second);
        }
        return maxCount;
    }
};

string reverseComplement(string_view text)
{
    auto text_upper = toUpper(string{text});
    static array<pair<char, char>, 4> table = {
        make_pair('A', 'T'),
        make_pair('T', 'A'),
        make_pair('C', 'G'),
        make_pair('G', 'C'),
    };

    string res;
    res.reserve(text_upper.size());
    for (auto it = text_upper.rbegin(); it != text_upper.rend(); it++) {
        for (const auto& t : table) {
            if (t.first == *it)
                res.push_back(t.second);
        }
    }

    assert(res.size() == text_upper.size());
    return res;
}

vector<int> findLocationsOfPattern(string_view pattern, string_view text)
{
    vector<int> res;
    const int n = pattern.size();
    if (text.size() < n)
        return res;
    for (int i = 0; i < text.size() - pattern.size() + 1; i++) {
        if (stringsEqual(pattern, 0, text, i, n)) {
            res.push_back(i);
        }
    }
    return res;
}

/**
 * d: hamming distance, the number of differences from pattern that can still be
 * considered equal to pattern
 */
vector<int> findLocationsOfPatternApprox(string_view pattern, string_view text,
                                         int d)
{
    vector<int> res;
    if (pattern.length() > text.size())
        return res;
    for (int i = 0; i < text.size() - pattern.length() + 1; ++i) {
        string_view curr(text.begin() + i, pattern.length());
        if (hammingDistance(pattern, curr) <= d) {
            res.push_back(i);
        }
    }
    return res;
}

/**
 * text: full genome to search
 * k: k-mer, length of piece of string
 * L: window length
 * t: minimum frequency count needed to be included in the result set
 */
vector<string> findClumps(string_view text, int k, int L, int t)
{
    unordered_set<string> patterns;
    int n = text.size();
    for (int i = 0; i < n - L + 1; i++) {
        string_view window(text.begin() + i, L);
        auto freqTable = FrequentWords::buildFrequencyTable(window, k);
        for (const auto& patternAndFreq : freqTable) {
            if (patternAndFreq.second >= t) {
                patterns.insert(patternAndFreq.first);
            }
        }
    }
    return vector<string>(patterns.begin(), patterns.end());
}

vector<int> skew(string_view text)
{
    vector<int> res(text.size() + 1, 0);
    for (int i = 0; i < text.size(); i++) {
        res[i + 1] = res[i];
        if (text[i] == 'G') {
            res[i + 1]++;
        } else if (text[i] == 'C') {
            res[i + 1]--;
        }
    }
    return res;
}

pair<int, vector<int>> minimumSkew(string_view text)
{
    auto skews = skew(text);
    pair<int, vector<int>> res;
    int mnm = numeric_limits<int>::max();
    vector<int> currLocs;
    for (int i = 0; i < skews.size(); i++) {
        auto s = skews[i];
        if (s <= mnm) {
            if (s < mnm) {
                currLocs.clear();
                mnm = s;
            }
            currLocs.push_back(i);
        }
    }
    return make_pair(mnm, currLocs);
}

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;
using ull = unsigned long long;

bool StringsEqual(string_view str1, int idx1, string_view str2, int idx2, int n)
{
    assert((idx1 + n <= str1.length()) && (idx2 + n <= str2.length()));
    assert(n > 0);
    for (int i = 0; i < n; i++) {
        if (str1[idx1 + i] != str2[idx2 + i]) return false;
    }
    return true;
}

int HammingDistance(string_view s1, string_view s2)
{
    int mlen = s1.length() <= s2.length() ? s1.length() : s2.length();
    int d = 0;
    for (int i = 0; i < mlen; i++) {
        if (s1[i] != s2[i]) d++;
    }
    d += abs(static_cast<long long>(s1.length() - s2.length()));
    return d;
}

int PatternCount(string_view text, string_view pattern)
{
    int count = 0;
    if (pattern.size() > text.size()) return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if (StringsEqual(text, i, pattern, 0, pattern.size())) count++;
    }
    return count;
}

int PatternCountApprox(string_view text, string_view pattern, int d)
{
    int count = 0;
    if (pattern.size() > text.size()) return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if (HammingDistance(string_view(text.begin() + i, pattern.size()),
                            pattern) <= d)
            count++;
    }
    return count;
}

class FrequentWords {
public:
    static pair<int, vector<string>> WithMismatches(string_view text, int k,
                                                    int d)
    {
        (void)text;
        (void)k;
        (void)d;
        return {0, {}};
    }

    static pair<int, vector<string>> ExactMatches(string_view text, int k)
    {
        vector<string> res;
        unordered_map<string, int> frequencyTable =
            BuildFrequencyTable(text, k);
        int maxOccurence = MaxOccurenceCount(frequencyTable);
        for (const auto& p : frequencyTable) {
            if (p.second == maxOccurence) {
                res.push_back(p.first);
            }
        }
        return make_pair(maxOccurence, res);
    }
    static unordered_map<string, int> BuildFrequencyTable(string_view text,
                                                          int k)
    {
        unordered_map<string, int> table;
        if (text.size() < k) return table;
        for (size_t i = 0; i <= text.size() - k; i++) {
            table[string(text.begin() + i, text.begin() + i + k)]++;
        }
        return table;
    }

private:
    static int MaxOccurenceCount(
        const unordered_map<string, int>& frequencyTable)
    {
        int maxCount = 0;
        for (auto& p : frequencyTable) {
            maxCount = max(maxCount, p.second);
        }
        return maxCount;
    }
};

string ReverseComplement(string_view text)
{
    static array<pair<char, char>, 4> table = {
        make_pair('A', 'T'),
        make_pair('T', 'A'),
        make_pair('C', 'G'),
        make_pair('G', 'C'),
    };

    string res;
    res.reserve(text.size());
    for (auto it = text.rbegin(); it != text.rend(); it++) {
        for (const auto& t : table) {
            if (t.first == *it) res.push_back(t.second);
        }
    }

    assert(res.size() == text.size());
    return res;
}

vector<int> FindPatternIndexes(string_view pattern, string_view text)
{
    vector<int> res;
    const int n = pattern.size();
    if (text.size() < n) return res;
    for (int i = 0; i < text.size() - pattern.size() + 1; i++) {
        if (StringsEqual(pattern, 0, text, i, n)) {
            res.push_back(i);
        }
    }
    return res;
}

/**
 * d: hamming distance, the number of differences from pattern that can still be
 * considered equal to pattern
 */
vector<int> FindPatternIndexesApprox(string_view pattern, string_view text,
                                     int d)
{
    vector<int> res;
    int patLen = pattern.length();
    if (patLen > text.size()) return res;
    for (int i = 0; i < text.size() - patLen + 1; ++i) {
        string_view curr(text.begin() + i, patLen);
        if (HammingDistance(pattern, curr) <= d) {
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
vector<string> FindClumps(string_view text, int k, int L, int t)
{
    unordered_set<string> patterns;
    int n = text.size();
    for (int i = 0; i < n - L + 1; i++) {
        string_view window(text.begin() + i, L);
        auto freqTable = FrequentWords::BuildFrequencyTable(window, k);
        for (const auto& patternAndFreq : freqTable) {
            if (patternAndFreq.second >= t) {
                patterns.insert(patternAndFreq.first);
            }
        }
    }
    return vector<string>(patterns.begin(), patterns.end());
}

vector<int> Skew(string_view text)
{
    vector<int> res(text.size() + 1, 0);
    for (int i = 0; i < text.size(); i++) {
        res[i + 1] = res[i];
        if (text[i] == 'G') {
            res[i + 1]++;
        }
        else if (text[i] == 'C') {
            res[i + 1]--;
        }
    }
    return res;
}

pair<int, vector<int>> MinimumSkew(string_view text)
{
    auto skews = Skew(text);
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

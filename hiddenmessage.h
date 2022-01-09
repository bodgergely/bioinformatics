#pragma once

#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <functional>
#include <array>

namespace hiddenmessage
{

using namespace std;
using ull=unsigned long long;

bool stringsEqual(string_view str1, int idx1, string_view str2, int idx2, int n)
{
    for(int i=0;i<n;i++) {
        if(str1[idx1 + i] != str2[idx2 + i]) 
            return false;
    }
    return true;
}

int patternCount(string_view text, string_view pattern)
{
    int count = 0;
    if(pattern.size() > text.size()) return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if(stringsEqual(text, i, pattern, 0, pattern.size()))
            count++;
    }
    return count;
}

class FrequentWords
{
public:
    static pair<int, unordered_set<string>>
    frequentWords(string_view text, int k)
    {
        unordered_set<string> res;
        unordered_map<string, int> frequencyTable = buildFrequencyTable(text, k);
        int maxOccurence = maxOccurenceCount(frequencyTable);
        for(const auto& p : frequencyTable) {
            if(p.second == maxOccurence) {
                res.insert(p.first);
            }
        }
        return make_pair(maxOccurence, res);
    }

    static unordered_map<string, int>
    buildFrequencyTable(string_view text, int k)
    {
        unordered_map<string, int> table;
        if(text.size() < k) return table;
        for (size_t i = 0; i <= text.size() - k; i++) {
            table[string(text.begin() + i, text.begin() + i + k)]++;
        }
        return table;
    }
private:
    static int 
    maxOccurenceCount(const unordered_map<string, int>& frequencyTable)
    {
        int maxCount = 0;
        for(auto& p : frequencyTable) {
            maxCount = max(maxCount, p.second);
        }
        return maxCount;
    }
};


string reverseComplement(string_view text)
{
    static array<pair<char, char>, 4> table = {
        make_pair('A', 'T'),
        make_pair('T', 'A'),
        make_pair('C', 'G'),
        make_pair('G', 'C'),
    };

    string res;
    res.reserve(text.size());
    for(auto it=text.rbegin(); it!=text.rend();it++) {
        for(const auto& t : table) {
            if(t.first == *it)
                res.push_back(t.second);
        }
    }

    assert(res.size() == text.size());
    return res;
}

vector<size_t> findPatternIndexes(string_view pattern, string_view text)
{
    vector<size_t> res;
    const int n = pattern.size();
    if(text.size() < n) return res;
    for(int i=0;i<text.size() - pattern.size()+1;i++) {
        if(stringsEqual(pattern, 0, text, i, n)) {
            res.push_back(i);
        }
    }
    return res;
}

vector<string> findClumps(string_view text, int k, int L, int t)
{
    unordered_set<string> patterns;
    int n = text.size();
    cout << "n:" << n << endl;
    for(int i=0;i<n - L + 1;i++) {
        string_view window(text.begin()+i, L);
        if(i % 1000 == 0) {
            cout << i << " out of " << n << " " << (i / (double)n) * 100 << "%" << endl;
        }
        auto freqTable = FrequentWords::buildFrequencyTable(window, k);
        for(const auto& patternAndFreq : freqTable) {
            if(patternAndFreq.second >= t) {
                patterns.insert(patternAndFreq.first);
            }
        }
    }
    return vector<string>(patterns.begin(), patterns.end());
}

}
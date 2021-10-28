/*
 * hiddenmessage.cpp
 *
 *  Created on: Oct 27 2021
 *      Author: Gergely Bod
 */

#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <unordered_map>

#include "rdtsc.h"

using namespace std;
using ull=unsigned long long;
ull cyclesPerMicrosec = 2612;

bool stringsEqual(const string& str1, int idx1, const string& str2, int idx2, int n)
{
    for(int i=0;i<n;i++) {
        if(str1[idx1 + i] != str2[idx2 + i]) 
            return false;
    }
    return true;
}

int patternCount(const string& text, const string& pattern)
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
    static unordered_set<string>
    frequentWords(const string& text, int k)
    {
        unordered_set<string> res;
        unordered_map<string, int> frequencyTable = buildFrequencyTable(text, k);
        int maxOccurence = maxOccurenceCount(frequencyTable);
        for(const auto& p : frequencyTable) {
            if(p.second == maxOccurence) {
                res.insert(p.first);
            }
        }
        return res;
    }
private:
    static unordered_map<string, int>
    buildFrequencyTable(const string& text, int k)
    {
        unordered_map<string, int> table;
        if(text.size() < k) return table;
        for (size_t i = 0; i <= text.size() - k; i++) {
            table[string(text.begin() + i, text.begin() + i + k)]++;
        }
        return table;
    }

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


int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) 
{
    string text;
    getline(cin, text);
    int k;
    cin >> k;

    Timer timer(cyclesPerMicrosec);
    timer.start();
    auto res = FrequentWords::frequentWords(text, k);
    for(auto& v : res) {
        cout << v << endl;
    }

    return 0;
}

void testAll()
{
}
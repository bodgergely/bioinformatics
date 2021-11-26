/*
 * hiddenmessage.cpp
 *
 *  Created on: Oct 27 2021
 *      Author: Gergely Bod
 */

#include <iostream>
#include <thread>
#include <array>
#include <cstdio>
#include <string>
#include <vector>
#include <utility>
#include <numeric>
#include <algorithm>
#include <cassert>
#include <unordered_set>
#include <unordered_map>

#include "rdtsc.h"
#include "utils/genomeGenerator.h"

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
    static pair<unordered_set<string>, int>
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
        return make_pair(res, maxOccurence);
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

void kmerFrequencyChance()
{
    int genomeLength = 500;
    int k = 9;
    int gotLucky = 0;
    int lucksNeeded = 5;
    int repeteCount = 4;
    ull iter = 0;
    for(iter=0;gotLucky < lucksNeeded;iter++) {
        string text = generateRandomGenome(genomeLength);
        auto res = FrequentWords::frequentWords(text, k);
        if(res.second >= repeteCount) {
            gotLucky++;
            printf("Got lucky (%d) at iter: %llu\n", gotLucky, iter);
        }
    }
    printf("At genomeLen: %d, %d-mer to be repeated at least: %d, the CHANCE is -> %f\n",
                    genomeLength, k, repeteCount, iter / (double)lucksNeeded);
}

string reverseComplement(const string& text)
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

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) 
{
    string text;
    getline(cin, text);
    cout << reverseComplement(text) << endl;
    return 0;
}

void testAll()
{
}

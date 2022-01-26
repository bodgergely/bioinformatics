#pragma once

#include "hiddenmessage.h"
#include "utils/genomeGenerator.h"
#include <functional>


namespace hiddenmessage
{

using namespace std;

// ull cyclesPerMicrosec = 2612; // on HP laptop

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
        if(res.first >= repeteCount) {
            gotLucky++;
            printf("Got lucky (%d) at iter: %llu\n", gotLucky, iter);
        }
    }
    printf("At genomeLen: %d, %d-mer to be repeated at least: %d, the CHANCE is -> %f\n",
                    genomeLength, k, repeteCount, iter / (double)lucksNeeded);
}

string replaceMostFrequent(const string& text, int k, std::function<string(const string&)> oper=
                                                        [](const string& s) {
                                                            string ns{s};
                                                            for_each(ns.begin(), ns.end(),
                                                                ::toupper);
                                                            return ns;
                                                        })
{
    if(text.size() < k) {
        return text;
    }
    auto mostFrequents = FrequentWords::frequentWords(text, k).second;
    string res{""};
    for(int i=0;i<text.size()-k+1;i++) {
        string curr(text.begin() + i, text.begin() + i + k);
        if(mostFrequents.find(curr) != mostFrequents.end()) {
            res += oper(curr);
            i += k;
        }
        else {
            res.push_back(text[i]);
        }
    }
    return res;
}

string highlight(string_view genome, vector<string> patterns, std::function<char(char)> func)
{
    vector<pair<int,int>> idxAndLen;
    for(auto& pattern : patterns) {
        for(auto& f : findPatternIndexes(pattern, genome)) {
            idxAndLen.push_back(make_pair(f, pattern.length()));
        }
    }
    string res = genome.data();
    int modifiedUntil = -1;
    for(int i=0;i<genome.size();i++) {
        for(auto& p : idxAndLen) {
            if(p.first == i) {
                modifiedUntil = i + p.second - 1;
                for(int j=i;j<=modifiedUntil;j++) {
                    res[j] = func(genome[j]);
                }
                break;
            }
        }
    }
    return res;
}

}
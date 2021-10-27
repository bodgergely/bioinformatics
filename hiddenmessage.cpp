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
#include <cassert>

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
    int patSum = accumulate(pattern.begin(), pattern.end(), 0);
    int currSum = 0;
    for (size_t i = 0; i < text.size(); i++) {
        currSum += text[i];
        if(i >= pattern.size()) {
            currSum -= text[i - pattern.size()];
        }
        // cout << "At index: " << i << " pat sum is: " << patSum << " curr sum is: " << currSum << endl;
        if(i >= pattern.size() - 1 && currSum == patSum && stringsEqual(text, i - (pattern.size() - 1), pattern, 0, pattern.size())) {
            // cout << "Found pattern at index: " << i << endl;
            count++;
        }
                    
    }
    // cout << count << endl;
    return count;
}

int patternCountSlow(const string& text, const string& pattern)
{
    int count = 0;
    if(pattern.size() > text.size()) return 0;
    for (size_t i = 0; i <= text.size() - pattern.size(); i++) {
        if(stringsEqual(text, i, pattern, 0, pattern.size()))
            count++;
    }
    return count;
}

using PatternCountFunc = int(const string&, const string&);

void testPatternCount(PatternCountFunc func)
{
    string t = "CGATATATCCATAG";
    string pat = "ATA";
    assert(func(t, pat) == 3);
    t = "A";
    pat = "VVV";
    assert(func(t, pat) == 0);
    t = "VVVV";
    pat = "VVV";
    assert(func(t, pat) == 2);
}

void testAll()
{
    testPatternCount(patternCount);
    testPatternCount(patternCountSlow);
}

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) 
{
    string text;
    getline(cin, text);
    string pat;
    getline(cin, pat);

    cout << "text len: " << text.size() << endl;
    cout << "pattern len: " << pat.size() << endl;

    Timer timer(cyclesPerMicrosec);
    timer.start();
    cout << patternCount(text, pat) << endl;
    cout << "hasher comparator took: " << timer.microsecs(timer.stop()) << endl;
    timer.start();
    cout << patternCountSlow(text, pat) << endl;
    cout << "brute force comparator took: "  << timer.microsecs(timer.stop()) << endl;

    return 0;
}
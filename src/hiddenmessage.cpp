/*
 * hiddenmessage.cpp
 *
 *  Created on: Oct 27 2021
 *      Author: Gergely Bod
 */

#include "hiddenmessage.h"

#include <array>

using namespace std;

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    string text;
    getline(cin, text);
    int k, L, t;
    cin >> k >> L >> t;

    for (auto& c : findClumps(text, k, L, t)) {
        cout << c << " ";
    }
    cout << "\n";
    for (auto i : findLocationsOfPattern("CCA", text)) {
        cout << i << " ";
    }

    return 0;
}
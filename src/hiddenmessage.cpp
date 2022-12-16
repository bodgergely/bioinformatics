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
    string line1;
    getline(cin, line1);

    string line2;
    getline(cin, line2);

    int d;
    cin >> d;

    for (auto i : findLocationsOfPatternApprox(line1, line2, d)) {
        cout << i << " ";
    }

    return 0;
}
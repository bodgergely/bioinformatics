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
    string pattern;
    getline(cin, text);
    getline(cin, pattern);

    cout << patternCount(text, pattern) << endl;

    return 0;
}
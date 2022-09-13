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
    int k, d;
    getline(cin, text);
    cin >> k >> d;

    FrequentWords::WithMismatches(text, k, d);

    return 0;
}
/*
 * hiddenmessage.cpp
 *
 *  Created on: Oct 27 2021
 *      Author: Gergely Bod
 */


#include "hiddenmessage.h"
#include <array>

using namespace hiddenmessage;
using namespace std;

int main([[maybe_unused]]int argc, [[maybe_unused]]char** argv) 
{
    // string text;
    // getline(cin, text);
    string s1, s2;
    int d;
    getline(cin, s1);
    getline(cin, s2);
    cin >> d;


    cout << patternCountApprox(s2, s1, d);


    return 0;
}
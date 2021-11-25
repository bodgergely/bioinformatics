/*
 * genomeGenerator.cpp
 *
 *  Created on: Apr 29, 2017
 *      Author: geri
 */
#include <iostream>
#include <string>
#include "genomeGenerator.h"

using namespace std;


int main(int argc, char** argv)
{
	if(argc < 2)
	{
		cout << "Needs size argument. Exiting\n";
		return -1;
	}

	int size = atoi(argv[1]);

	string buff = generateRandomGenome(size);
	cout << buff;
	cout << "\n";
	return 0;
}




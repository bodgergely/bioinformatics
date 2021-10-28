/*
 * genomeGenerator.cpp
 *
 *  Created on: Apr 29, 2017
 *      Author: geri
 */
#include <iostream>
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
	const char* file = argv[2];

	char* buff = generate(size);

	ofstream os(file);
	if(os.is_open())
	{
		os << buff;
		os << "\n";
		os.close();
	}
	else
	{
		cout << "Error opening file: " << file << endl;
		exit(1);
	}

	return 0;

}




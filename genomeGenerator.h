#pragma once
#include <ctime>
#include <cstdlib>
#include <fstream>
char* generate(int size)
{
	char alphabet[] = {'A', 'C', 'G', 'T'};
	char* buffer = new char[size + 1];
	char* pb = buffer;
	srand(time(NULL));
	for(int i=0;i<size;i++)
	{
		*pb++ = alphabet[rand() % 4];
	}
	*pb = '\0';

	return buffer;
}


#pragma once
#include <string>
#include <iostream>
#include <random>

std::string generateRandomGenome(int size)
{
	char alphabet[] = {'A', 'C', 'G', 'T'};
	std::random_device device;
	std::default_random_engine engine(device());
	std::uniform_int_distribution dist(0, (int)(sizeof(alphabet) / sizeof(alphabet[0])) - 1);
	std::string res;
	for(int i=0;i<size;i++)
	{
		auto idx = dist(engine); 
		res.push_back(alphabet[idx]);
	}

	return res;
}

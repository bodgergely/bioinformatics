/*
 * hiddenmessage.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: geri
 */

#include "rdtsc.h"
#include "genomeGenerator.h"

#include <string>
#include <cstring>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>

#include <sys/mman.h>

using namespace std;

double avgCyclesPerMicroSec = 2394.74;

int patternCount(const string& text, const char* pattern, int plen)
{
	int c = 0;
	for(int i=0;i<text.size()-plen;i++)
	{
		if(!memcmp(text.c_str() + i, pattern, plen))
		{
			c++;
		}
	}
	return c;
}

inline char complement(char s)
{
	switch(s)
	{
	case 'A':
		return 'T';
	case 'T':
		return 'A';
	case 'C':
		return 'G';
	case 'G':
		return 'C';
	}

	throw std::runtime_error("It is not a valid character" + s);
}

string reverseComplement(const string& s)
{

	string r;
	r.reserve(s.size());
	for(int i=s.size()-1;i>=0;i--)
	{
		r.push_back(complement(s[i]));
	}
	return r;
}

vector<string> mostFrequentWords_N2(const string& text, int k)
{
	int* count = new int[text.size() - k + 1];
	int maxc = 0;
	for(int i=0;i<text.size() - k;i++)
	{
		count[i] = patternCount(text, text.c_str() + i, k);
		if(count[i] > maxc)
			maxc = count[i];
	}

	vector<string> mfs;
	for(int i=0;i<text.size() - k;i++)
	{
		if(count[i] == maxc)
		{
			mfs.push_back(string(text.c_str() + i, k));
		}
	}

	vector<string> res;
	for(int i=0;i<mfs.size();i++)
	{
		if(find(res.begin(), res.end(), mfs[i]) == res.end())
			res.push_back(mfs[i]);
	}

	return res;
}

static int offset(char c, int pos)
{
	static const int alphabet[] = {'A', 'C', 'G', 'T'};
	int i=0;
	for(;i<4;i++)
	{
		if(c == alphabet[i])
			break;
	}

	int offs = i * pow(4, pos);
	return offs;
}

int patternToNumber(const char* s, int k)
{
	int loc = 0;
	for(int i=0;i<k;i++)
	{
		loc+=offset(s[i], k - i - 1);
	}
	return loc;
}

string numToPattern(int pos, int k)
{
	static const int alphabet[] = {'A', 'C', 'G', 'T'};
	int f = pos;
	string s;
	for(int i=k-1;i>=0;i--)
	{
		int t = pow(4, i);
		int l = f/t;
		s.push_back(alphabet[l]);
		f = f % t;
	}

	return s;
}

void patternTester(const char* text, int k)
{
	for(int i=0;i<strlen(text)-k + 1;i++)
	{
		string orig = string(text + i, k);
		int num = patternToNumber(text + i, k);
		string pat = numToPattern(num, k);
		cout << "At " << i << " : " << orig << " " << pat << endl;
		if(orig!=pat)
		{
			printf("orig: %s at %d which is encoded to %d does not equal decoded: %s\n", orig.c_str(), i, num, pat.c_str());
			while(1);
		}
	}
}


void printFreqArray(int* array, int size)
{
	for(int i=0;i<size;i++)
	{
		cout << array[i];
		if(i < size-1)
			cout << " ";
	}
	//cout << "\n";
}

vector<string> freqArray(const string& text, int k)
{
	int  kmerCount = pow(4, k);
	int* array = new int[kmerCount];
	memset(array, 0, sizeof(int) * kmerCount);
	int maxc = 0;
	for(int i=0;i<text.size()-k + 1;i++)
	{
		int loc = patternToNumber(text.c_str()+i, k);
		//cout << loc << " ";
		array[loc]++;
		if(array[loc] > maxc)
			maxc = array[loc];
	}

	vector<string> res;
	for(int i=0;i<kmerCount;i++)
	{
		if(array[i] == maxc)
		{
			string p = numToPattern(i, k);
			res.push_back(p);
		}
	}

	//cout << "Frequency array: " << endl;
	//printFreqArray(array, kmerCount);

	return res;
}

vector<string> freqArraySorted(const string& text, int k)
{
	vector<int> coded;
	coded.reserve(text.size()-k+1);
	for(int i=0;i<text.size()-k + 1;i++)
	{
		int loc = patternToNumber(text.c_str()+i, k);
		coded.push_back(loc);
	}

	//ull s = rdtsc();
	sort(coded.begin(), coded.end());
	//ull e = rdtsc();
	//printf("coded size: %lu sort took: %f\n", coded.size(), (e - s)/ avgCyclesPerMicroSec);

	vector<int> maxLocs;
	int prev = coded[0];
	int maxc = 1;
	int counter = 1;
	int currLoc = 0;
	for(int i=1;i<coded.size();i++)
	{
		if(coded[i]!=prev)
		{
			if(counter > maxc)
			{
				maxc = counter;
				maxLocs.clear();
				maxLocs.push_back(coded[currLoc]);
			}
			else if(counter == maxc)
			{
				maxLocs.push_back(coded[currLoc]);
			}
			currLoc = i;
			counter = 1;
			prev = coded[i];
		}
		else
		{
			counter++;
		}
	}

	vector<string> res;
	for(int l : maxLocs)
	{
		res.push_back(numToPattern(l, k));
	}

	return res;

}


vector<string> freqArraySet(const string& text, int k)
{
	unordered_map<int, int> words;
	words.reserve(text.size()-k + 1);

	for(int i=0;i<text.size()-k+1;i++)
	{
		words[patternToNumber(text.c_str()+i, k)]++;
	}

	int maxC = 0;
	vector<string> maxwords;
	for(auto it = words.begin();it != words.end();it++)
	{
		if(it->second > maxC)
		{
			maxC = it->second;
			maxwords.clear();
			maxwords.push_back(numToPattern(it->first, k));
		}
		else if(it->second == maxC)
			maxwords.push_back(numToPattern(it->first, k));
	}

	return maxwords;

}



typedef vector<string> (*freq)(const string& text, int k) ;

double measure(freq fun, const string& text, int k)
{
	ull s = rdtsc();
	fun(text, k);
	ull e = rdtsc();
	return (e-s)/ float(avgCyclesPerMicroSec);
}


struct measures
{
	int kmer;
	int textSize;
	double f;
	double s;
	double t;
};

vector<string> dummy(const string& text, int k)
{
	return vector<string>();
}

vector<measures> competition()
{
	vector<measures> ms;
	for(int k = 10;k<25;k++)
	{
		int textSize = 500;
		for(int i=0;i<3;i++)
		{
			char* text = generate(textSize);
			measures m;
			m.kmer = k;
			m.textSize = textSize;
			m.f = measure(dummy, text, k);
			m.s = measure(freqArraySorted, text, k);
			m.t = measure(freqArraySet, text, k);
			ms.push_back(m);
			textSize *= 100;
			delete[] text;
			printf("%f, %f, %f, k: %d, size: %d\n", m.f, m.s, m.t, m.kmer, m.textSize);
		}

	}

	return ms;

}



class Genome
{
public:
	Genome(const string& text) : _genome(text) {}
	vector<string> findMostFreqKmers(int k)
	{
		//void* mem = mmap(NULL, text.size()-k +1, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_POPULATE|MAP_ANONYMOUS|MAP_HUGE_TLB|MAP_HUGE_2MB)
		vector<int> coded;
		const string& text = _genome;
		coded.reserve(text.size()-k+1);
		for(int i=0;i<text.size()-k + 1;i++)
		{
			int loc = patternToNumber(text.c_str()+i, k);
			coded.push_back(loc);
		}

		//ull s = rdtsc();
		sort(coded.begin(), coded.end());
		cout << getpagesize() << endl;
		//ull e = rdtsc();
		//printf("coded size: %lu sort took: %f\n", coded.size(), (e - s)/ avgCyclesPerMicroSec);

		vector<int> maxLocs;
		int prev = coded[0];
		int maxc = 1;
		int counter = 1;
		int currLoc = 0;
		for(int i=1;i<coded.size();i++)
		{
			if(coded[i]!=prev)
			{
				if(counter > maxc)
				{
					maxc = counter;
					maxLocs.clear();
					maxLocs.push_back(coded[currLoc]);
				}
				else if(counter == maxc)
				{
					maxLocs.push_back(coded[currLoc]);
				}
				currLoc = i;
				counter = 1;
				prev = coded[i];
			}
			else
			{
				counter++;
			}
		}

		vector<string> res;
		for(int l : maxLocs)
		{
			res.push_back(numToPattern(l, k));
		}

		return res;

	}
	vector<int> findOccurences(const string& pattern)
	{
		vector<int> occurences;
		int k = pattern.size();
		for(int i=0;i<_genome.size() - k + 1;i++)
		{
			if(!memcmp(_genome.c_str() + i, pattern.c_str(), k))
				occurences.push_back(i);
		}
		return occurences;
	}
private:
	const string& _genome;
};


int main()
{
	string text;
	//string pattern = "CTTGATCAT";
	//getline(std::cin, pattern);
	getline(std::cin, text);

	Genome genome(text);
	ull s = rdtsc();
	vector<string> kmers = genome.findMostFreqKmers(9);
	ull e = rdtsc();
	printf("Took secs: %f\n", (e-s)/avgCyclesPerMicroSec/1000000);
	for(const string& k : kmers)
	{
		cout << k << " ";
	}
	cout << endl;
	/*vector<string> kmers = {"CTTGATCAT", "ATGATCAAG"};

	for(string& kmer : kmers)
	{
		cout << "kmer occurences: " << kmer << endl;
		vector<int> res = genome.findOccurences(kmer);
		for(int i=0;i<res.size();i++)
		{
			cout << res[i];
			if(i < res.size()-1)
				cout << " ";
		}
		cout << "\n";
	}*/

	return 0;
}




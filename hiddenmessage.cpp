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
#include <unordered_set>

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

	delete[] count;

	return res;
}

static inline int offset(char c, int pos)
{
	static const int alphabet[] = {'A', 'C', 'G', 'T'};
	int i=0;
	for(;i<4;i++)
	{
		if(c == alphabet[i])
			break;
	}
	return i * (0x01 << (pos*2));
}

inline int patternToNumber(const char* s, int k)
{
	int loc = 0;
	for(int i=0;i<k;i++)
	{
		loc+=offset(s[i], k - i - 1);
	}
	return loc;
}

inline int fastNextPos(int prevpos, int k, int nextval)
{
	return ((prevpos % (1 << 2*(k-1))) << 2) +  nextval;
}


string numToPattern(int pos, int k)
{
	static const int alphabet[] = {'A', 'C', 'G', 'T'};
	int f = pos;
	string s;
	for(int i=k-1;i>=0;i--)
	{
		int t = (0x01 << (i*2));
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
	int  kmerCount = (0x01 << (k*2));
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
	delete[] array;
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


pair<vector<string>, int> findMostFreqKmers(const char* start, uint64_t size, int k)
{
	vector<int> coded;
	const char* text = start;
	coded.reserve(size-k+1);
	for(int i=0;i<size-k + 1;i++)
	{
		int loc = patternToNumber(text+i, k);
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

	return make_pair(res, maxc);

}



vector<uint64_t> findOccurences(const char* pattern, int patternLen, const char* text, uint64_t textLen)
{
	vector<uint64_t> occurences;
	int k = patternLen;
	for(uint64_t i=0;i<textLen - k + 1;i++)
	{
		if(!memcmp(text + i, pattern, k))
			occurences.push_back(i);
	}
	return occurences;
}




class Genome
{
public:
	Genome(const string& text) : _genome(text)
	{
		_nucleoLookup['A'] = 0;
		_nucleoLookup['C'] = 1;
		_nucleoLookup['G'] = 2;
		_nucleoLookup['T'] = 3;
	}
	pair<vector<string>, int> findMostFreqKmers(int k) const
	{
		return ::findMostFreqKmers(_genome.c_str(), _genome.size(), k);
	}


	vector<string> clumpsNaive(int k, int L, int t)
	{

		int  kmerCount = (0x01 << (k*2));
		int* clump = new int[kmerCount];
		memset(clump, 0, sizeof(int) * kmerCount);
		const char* text = _genome.c_str();
		for(int i=0;i<_genome.size()-L+1;i++)
		{
			cout << i << "\n";
			int* a = new int[kmerCount];
			memset(a, 0, sizeof(int) * kmerCount);
			for(int i=0;i<L-k + 1;i++)
			{
				int loc = patternToNumber(text + i, k);
				//cout << loc << " ";
				a[loc]++;
			}

			for(int i=0;i<kmerCount;i++)
			{
				if(a[i] >= t)
				{
					clump[i] = 1;
				}
			}
			delete[] a;
		}

		vector<string> res;
		for(int i=0;i<kmerCount;i++)
		{
			if(clump[i] == 1)
			{
				string p = numToPattern(i, k);
				res.push_back(p);
			}
		}
		delete[] clump;

		return res;
	}
	vector<string> clumps(int k, int L, int t, uint64_t searchwindowSize=0, uint64_t startpos=0)
	{
		if(searchwindowSize == 0)
			searchwindowSize = _genome.size();

		int  kmerCount = (0x01 << (k*2));
		char* clump = new char[kmerCount];
		memset(clump, 0, sizeof(char) * kmerCount);
		uint64_t end = searchwindowSize;
		const char* text = _genome.c_str() + startpos;

		if(startpos + searchwindowSize > _genome.size())
			searchwindowSize = _genome.size() - startpos;

		int* temp = new int[kmerCount];
		memset(temp, 0, sizeof(int) * kmerCount);
		int prevLoc = patternToNumber(text, k);
		int backLoc = prevLoc;
		temp[prevLoc]++;
		if(temp[prevLoc] >= t)
			clump[prevLoc] = 1;

		for(int i=1;i<L - k + 1;i++)
		{
			int loc = fastNextPos(prevLoc, k, _nucleoLookup[text[i + k - 1]]);
			temp[loc]++;
			if(temp[loc] >= t)
				clump[loc] = 1;
			prevLoc = loc;
		}

		for(int i=1;i<end-L+1;i++)
		{
			temp[backLoc]--;
			backLoc = fastNextPos(backLoc, k, _nucleoLookup[text[i + k - 1]]);
			prevLoc = fastNextPos(prevLoc, k, _nucleoLookup[text[i + L - 1]]);
			temp[prevLoc]++;
			if(temp[prevLoc] >= t)
				clump[prevLoc] = 1;
		}

		delete[] temp;

		vector<string> res;
		for(int i=0;i<kmerCount;i++)
		{
			if(clump[i] == 1)
			{
				string p = numToPattern(i, k);
				res.push_back(p);
			}
		}
		delete[] clump;

		return res;
	}

	int skew(uint64_t i, uint64_t s=0)
	{
		if(s + i > _genome.size())
			return -1;

		int diff = 0;
		uint64_t end = s + i;
		for(uint64_t p=s;p<end;p++)
		{
			diff = _skew(diff, p);
		}

		return diff;
	}

	vector<int> minimumSkew() const
	{
		vector<int> res;
		int diff = 0;
		int min = diff;

		for(uint64_t i=0;i<_genome.size();i++)
		{
			diff = _skew(diff, i);

			if(diff < min)
			{
				min = diff;
				res.clear();
				res.push_back(i + 1);
			}
			else if(diff == min)
				res.push_back(i + 1);
			//cout << diff << "\n";
		}

		return res;
	}

	size_t size() const
	{
		return _genome.size();
	}

private:
	inline	int _skew(int before, uint64_t pos) const
	{
		if(_genome[pos] == 'G')
			before++;
		else if(_genome[pos] == 'C')
			before--;
		return before;
	}
private:
	const string& _genome;
	char 		 _nucleoLookup[256];
};


static char nucleos[] = {'A', 'C', 'G', 'T'};

int hammingDistance(const char* s1, const char* s2, int len)
{
	int diff = 0;
	for(int i=0;i<len;i++)
	{
		if(*s1++!=*s2++)
			diff++;
	}
	return diff;
}

vector<uint64_t> approximatePatternMatch(const char* pattern, int patternLen, const char* text, int textLen, int mismatchLimit)
{
	if(patternLen > textLen)
		return vector<uint64_t>();

	int k = patternLen;
	const char* textEnd = text+textLen;

	vector<uint64_t> res;
	for(const char* t=text;t<textEnd;t++)
	{
		if(hammingDistance(t, pattern, patternLen) <= mismatchLimit)
		{
			res.push_back(t - text);
		}
	}

	return res;
}

uint32_t approximatePatternCount(const char* pattern, int patternLen, const char* text, int textLen, int mismatchLimit)
{
	auto res =  approximatePatternMatch(pattern, patternLen, text, textLen, mismatchLimit);
	return res.size();
}


static void _generateSimilar(const char* pattern, int patternLen, int pos, int l, int mismatchLimit, char* buffer, int* table)
{
	if(pos == patternLen || l > mismatchLimit)
	{
		return;
	}
	for(int p=pos;p<patternLen;p++)
	{
		char c = pattern[p];
		for(int i=0;i<4;i++)
		{
			buffer[p] = nucleos[i];
			table[patternToNumber(buffer, patternLen)] = 1;
			_generateSimilar(pattern, patternLen, pos+1, l+1, mismatchLimit, buffer, table);
		}
		buffer[p] = c;
	}

}

void generateSimilar(const char* pattern, int patternLen, int mismatchLimit, int* table)
{
	char* buffer = new char[patternLen];
	memcpy(buffer, pattern, patternLen);
	table[patternToNumber(buffer, patternLen)] = 1;
	_generateSimilar(pattern, patternLen, 0, 1, mismatchLimit, buffer, table);
}


void computingFrequenciesWithMismatches(const char* text, uint64_t textLen, int k, int d, int* freqArray)
{
	memset(freqArray, 0, sizeof(int)*(0x1 < (2*k)));
	int ts = 1 << (2*k);
	int* m = new int[ts];
	for(int i=0;i<=textLen-k;i++)
	{
		memset(m, 0, sizeof(int)*ts);
		generateSimilar(text+i, k, d, m);
		for(int i=0;i<ts;i++)
			freqArray[i]+=m[i];
	}
	delete[] m;
}


/*
 * return the kmers( that differ with count d) with maxcount
 */

int* freqArray(int k)
{
	int freqArraySize = 1 << (2*k);
	int* freqArray = new int[freqArraySize];
	memset(freqArray, 0, sizeof(int)*freqArraySize);
	return freqArray;
}

pair<vector<string>, int> findMostFreqKmersWithMismatches(const char* text, uint64_t size, int k, int d, bool reverseIncluded)
{
	int freqArraySize = 1 << (2*k);
	int* fa = freqArray(k);
	computingFrequenciesWithMismatches(text, size, k, d, fa);
	int maxLoc = 0;
	vector<string> r;
	if(!reverseIncluded)
	{
		for(int i=0;i<freqArraySize;i++)
		{
			int f = fa[i];
			if(f > maxLoc)
				maxLoc = f;
		}
		for(int i=0;i<freqArraySize;i++)
		{
			if(maxLoc == fa[i])
			{
				r.push_back(numToPattern(i, k));
			}
		}
	}
	else
	{
		int* nfa = freqArray(k);
		bool* processed = new bool[freqArraySize];
		memset(processed, 0, sizeof(bool) * freqArraySize);
		for(int i=0;i<freqArraySize;i++)
		{
			if(!processed[i])
			{
				int g = fa[i];
				int reverseLoc = patternToNumber(reverseComplement(numToPattern(i, k)).c_str(), k);
				int h = fa[reverseLoc];
				nfa[i] = g+h;
				nfa[reverseLoc] = g+h;
				processed[i] = true;
				processed[reverseLoc] = true;

				if(g+h > maxLoc)
					maxLoc = g+h;
			}
		}

		for(int i=0;i<freqArraySize;i++)
		{
			if(nfa[i] == maxLoc)
			{
				r.push_back(numToPattern(i, k));
			}
		}

		delete[] nfa;
		delete[] processed;
	}
	delete[] fa;

	return make_pair(r, maxLoc);

}




/*
 *

 *
 */

int main()
{
	string text;
	//string text2;
	getline(cin, text);
	int k = 9;
	int d = 1;
	//cin >> k >> d;
	//string pattern =
	Genome genome(text);
	ull s1 = rdtsc();
	vector<int> skews = genome.minimumSkew();
	ull s2 = rdtsc();
	auto res = findMostFreqKmersWithMismatches(text.c_str() + skews[0], 500, k, d, true);
	ull s3 = rdtsc();
	cout << "count: " << res.second << "\n";
	for(string& s : res.first)
	{
		cout << s << " ";
	}
	cout << "Finished microsecs: " << ((s3-s1) / avgCyclesPerMicroSec) << endl;
	cout << "Finding skews: " << (s2 - s1) / avgCyclesPerMicroSec << " finding kmers: " << (s3 - s2)/avgCyclesPerMicroSec << endl;

	return 0;
}




/*
 * hiddenmessage.cpp
 *
 *  Created on: Apr 28, 2017
 *      Author: geri
 */

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "genomeGenerator.h"
#include "rdtsc.h"

using namespace std;

double avgCyclesPerMicroSec = 2905.74;

int patternCount(const string& text, const char* pattern, int plen)
{
    int c = 0;
    for (int i = 0; i < text.size() - plen; i++) {
        if (!memcmp(text.c_str() + i, pattern, plen)) {
            c++;
        }
    }
    return c;
}

inline char complement(char s)
{
    switch (s) {
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
    for (int i = s.size() - 1; i >= 0; i--) {
        r.push_back(complement(s[i]));
    }
    return r;
}

/*
* Frequency table for a given k length
*/
int* freqTable(int k)
{
    int tableSize = 1 << (2 * k); // number of elements in table
    int* table = new int[tableSize]; // allocate the table
    memset(table, 0, sizeof(int) * tableSize); // zero out the table
    return table;
}

vector<string> mostFrequentWords_N2(const string& text, int k)
{
    int* count = new int[text.size() - k + 1];
    int maxc = 0;
    for (int i = 0; i < text.size() - k; i++) {
        count[i] = patternCount(text, text.c_str() + i, k);
        if (count[i] > maxc)
            maxc = count[i];
    }

    vector<string> mfs;
    for (int i = 0; i < text.size() - k; i++) {
        if (count[i] == maxc) {
            mfs.push_back(string(text.c_str() + i, k));
        }
    }

    vector<string> res;
    for (int i = 0; i < mfs.size(); i++) {
        if (find(res.begin(), res.end(), mfs[i]) == res.end())
            res.push_back(mfs[i]);
    }

    delete[] count;

    return res;
}

static inline int offset(char c, int pos)
{
    static const int alphabet[] = {'A', 'C', 'G', 'T'};
    int i = 0;
    for (; i < 4; i++) {
        if (c == alphabet[i])
            break;
    }
    return i * (0x01 << (pos * 2));
}

inline int patternToNumber(const char* s, int k)
{
    int loc = 0;
    for (int i = 0; i < k; i++) {
        loc += offset(s[i], k - i - 1);
    }
    return loc;
}

inline int fastNextPos(int prevpos, int k, int nextval)
{
    return ((prevpos % (1 << 2 * (k - 1))) << 2) + nextval;
}

string numToPattern(int pos, int k)
{
    static const int alphabet[] = {'A', 'C', 'G', 'T'};
    int f = pos;
    string s;
    for (int i = k - 1; i >= 0; i--) {
        int t = (0x01 << (i * 2));
        int l = f / t;
        s.push_back(alphabet[l]);
        f = f % t;
    }

    return s;
}

void patternTester(const char* text, int k)
{
    for (int i = 0; i < strlen(text) - k + 1; i++) {
        string orig = string(text + i, k);
        int num = patternToNumber(text + i, k);
        string pat = numToPattern(num, k);
        cout << "At " << i << " : " << orig << " " << pat << endl;
        if (orig != pat) {
            printf(
                "orig: %s at %d which is encoded to %d does not equal decoded: "
                "%s\n",
                orig.c_str(), i, num, pat.c_str());
            while (1)
                ;
        }
    }
}

void printFreqArray(int* array, int size)
{
    for (int i = 0; i < size; i++) {
        cout << array[i];
        if (i < size - 1)
            cout << " ";
    }
    // cout << "\n";
}

vector<string> freqArrayTable(const string& text, int k)
{
    int kmerCount = (0x01 << (k * 2));
    int* array = new int[kmerCount];
    memset(array, 0, sizeof(int) * kmerCount);
    int maxc = 0;
    for (int i = 0; i < text.size() - k + 1; i++) {
        int loc = patternToNumber(text.c_str() + i, k);
        // cout << loc << " ";
        array[loc]++;
        if (array[loc] > maxc)
            maxc = array[loc];
    }

    vector<string> res;
    for (int i = 0; i < kmerCount; i++) {
        if (array[i] == maxc) {
            string p = numToPattern(i, k);
            res.push_back(p);
        }
    }

    // cout << "Frequency array: " << endl;
    // printFreqArray(array, kmerCount);
    delete[] array;
    return res;
}

/**
 * Returns the list of most frequent kmers
 * uses sorting to count
 */
vector<string> freqArraySorted(const string& text, int k)
{
    vector<int> coded;
    coded.reserve(text.size() - k + 1);
    for (int i = 0; i < text.size() - k + 1; i++) {
        int loc = patternToNumber(text.c_str() + i, k);
        coded.push_back(loc);
    }

    // ull s = __rdtsc();
    sort(coded.begin(), coded.end());
    // ull e = __rdtsc();
    // printf("coded size: %lu sort took: %f\n", coded.size(), (e - s)/
    // avgCyclesPerMicroSec);

    vector<int> maxLocs;
    int prev = coded[0];
    int maxc = 1;
    int counter = 1;
    int currLoc = 0;
    for (int i = 1; i < coded.size(); i++) {
        if (coded[i] != prev) {
            if (counter > maxc) {
                maxc = counter;
                maxLocs.clear();
                maxLocs.push_back(coded[currLoc]);
            } else if (counter == maxc) {
                maxLocs.push_back(coded[currLoc]);
            }
            currLoc = i;
            counter = 1;
            prev = coded[i];
        } else {
            counter++;
        }
    }

    vector<string> res;
    for (int l : maxLocs) {
        res.push_back(numToPattern(l, k));
    }

    return res;
}

/**
 * Returns the list of the most frequent kmers
 * uses unordered_map to count
 */
vector<string> freqArraySet(const string& text, int k)
{
    unordered_map<int, int> words;
    words.reserve(text.size() - k + 1);

    for (int i = 0; i < text.size() - k + 1; i++) {
        words[patternToNumber(text.c_str() + i, k)]++;
    }

    int maxC = 0;
    vector<string> maxwords;
    for (auto it = words.begin(); it != words.end(); it++) {
        if (it->second > maxC) {
            maxC = it->second;
            maxwords.clear();
            maxwords.push_back(numToPattern(it->first, k));
        } else if (it->second == maxC)
            maxwords.push_back(numToPattern(it->first, k));
    }

    return maxwords;
}

typedef vector<string> (*freq)(const string& text, int k);

double measure(freq fun, const string& text, int k)
{
    ull s = __rdtsc();
    fun(text, k);
    ull e = __rdtsc();
    return (e - s) / float(avgCyclesPerMicroSec);
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
    for (int k = 10; k < 25; k++) {
        int textSize = 500;
        for (int i = 0; i < 3; i++) {
            auto text = generateRandomGenome(textSize);
            measures m;
            m.kmer = k;
            m.textSize = textSize;
            m.f = measure(dummy, text.c_str(), k);
            m.s = measure(freqArraySorted, text.c_str(), k);
            m.t = measure(freqArraySet, text.c_str(), k);
            ms.push_back(m);
            textSize *= 100;
            printf("%f, %f, %f, k: %d, size: %d\n", m.f, m.s, m.t, m.kmer,
                   m.textSize);
        }
    }

    return ms;
}

/**
 * Returns a list of pairs of the most frequent kmer -> count of the pattern
 */
pair<vector<string>, int> findMostFreqKmers(const char* start, uint64_t size,
                                            int k)
{
    // idea: code the patterns and sort the coded patterns and count them and
    // return the max counted pattern(s) and the number of times that(those)
    // patterns are in the genome
    vector<int> coded;
    const char* text = start;
    coded.reserve(size - k + 1);
    for (int i = 0; i < size - k + 1; i++) {
        int loc = patternToNumber(text + i, k);
        coded.push_back(loc);
    }

    // ull s = __rdtsc();
    sort(coded.begin(), coded.end());
    // ull e = __rdtsc();
    // printf("coded size: %lu sort took: %f\n", coded.size(), (e - s)/
    // avgCyclesPerMicroSec);

    // now the coded vector is sorted and contains the coded patterns, iterate
    // over it and since it is sorted easy to count the patterns
    vector<int> maxLocs;
    int prev = coded[0];
    int maxc = 1;
    int counter = 1;
    int currLoc = 0;
    for (int i = 1; i < coded.size(); i++) {
        if (coded[i] != prev) {
            if (counter > maxc) {
                maxc = counter;
                maxLocs.clear();
                maxLocs.push_back(coded[currLoc]);
            } else if (counter == maxc) {
                maxLocs.push_back(coded[currLoc]);
            }
            currLoc = i;
            counter = 1;
            prev = coded[i];
        } else {
            // if still the same, count it
            counter++;
        }
    }

    vector<string> res;
    for (int l : maxLocs) {
        res.push_back(numToPattern(l, k));
    }

    return make_pair(res, maxc);
}

/**
 * Find the number of times 'pattern' occurs in 'text'
 */
vector<uint64_t> findOccurences(const char* pattern, int patternLen,
                                const char* text, uint64_t textLen)
{
    vector<uint64_t> occurences;
    int k = patternLen;
    for (uint64_t i = 0; i < textLen - k + 1; i++) {
        if (!memcmp(text + i, pattern, k))
            occurences.push_back(i);
    }
    return occurences;
}

int hammingDistance(const char* s1, const char* s2, int len)
{
    int diff = 0;
    for (int i = 0; i < len; i++) {
        if (*s1++ != *s2++)
            diff++;
    }
    return diff;
}

vector<uint64_t> approximatePatternMatch(const char* pattern, int patternLen,
                                         const char* text, int textLen,
                                         int mismatchLimit)
{
    if (patternLen > textLen)
        return vector<uint64_t>();

    const char* textEnd = text + textLen;

    vector<uint64_t> res;
    for (const char* t = text; t <= textEnd - patternLen; t++) {
        if (hammingDistance(t, pattern, patternLen) <= mismatchLimit) {
            res.push_back(t - text);
        }
    }

    return res;
}

uint32_t approximatePatternCount(const char* pattern, int patternLen,
                                 const char* text, int textLen,
                                 int mismatchLimit)
{
    auto res = approximatePatternMatch(pattern, patternLen, text, textLen,
                                       mismatchLimit);
    return res.size();
}

static void _generateSimilar(const char* pattern, int patternLen, int pos,
                             int l, int mismatchLimit, char* buffer, int* table)
{
    static const char nucleos[] = {'A', 'C', 'G', 'T'};
    if (pos == patternLen || l > mismatchLimit) {
        return;
    }
    for (int p = pos; p < patternLen; p++) {
        char c = pattern[p];
        for (int i = 0; i < 4; i++) {
            buffer[p] = nucleos[i];
            table[patternToNumber(buffer, patternLen)] = 1;
            _generateSimilar(pattern, patternLen, pos + 1, l + 1, mismatchLimit,
                             buffer, table);
        }
        buffer[p] = c;
    }
}

/**
 * Output: 'table' - allocated and owned by caller
 */
void generateSimilar(const char* pattern, int patternLen, int mismatchLimit,
                     int* table)
{
    char* buffer = new char[patternLen];
    memcpy(buffer, pattern, patternLen);
    table[patternToNumber(buffer, patternLen)] = 1;
    _generateSimilar(pattern, patternLen, 0, 1, mismatchLimit, buffer, table);
    delete[] buffer;
}

vector<string> generateSimilar(const char* pattern, int patternLen,
                               int mismatchLimit)
{
    int* table = freqTable(patternLen);
    int tableSize = 1 << (2 * patternLen);
    char* buffer = new char[patternLen];
    memcpy(buffer, pattern, patternLen);
    table[patternToNumber(buffer, patternLen)] = 1;
    _generateSimilar(pattern, patternLen, 0, 1, mismatchLimit, buffer, table);

    vector<string> res;
    for (int i = 0; i < tableSize; i++) {
        if (table[i])
            res.push_back(numToPattern(i, patternLen));
    }
    delete[] buffer;
    delete[] table;

    return res;
}

void computingFrequenciesWithMismatches(const char* text, uint64_t textLen,
                                        int k, int d, int* freqArray)
{
    memset(freqArray, 0, sizeof(int) * (0x1 < (2 * k)));
    int ts = 1 << (2 * k);
    int* m = new int[ts];
    for (int i = 0; i <= textLen - k; i++) {
        memset(m, 0, sizeof(int) * ts);
        generateSimilar(text + i, k, d, m);
        for (int i = 0; i < ts; i++) freqArray[i] += m[i];
    }
    delete[] m;
}

/*
 * return the kmers( that differ with count d) with maxcount
 */
pair<vector<string>, int> findMostFreqKmersWithMismatches(const char* text,
                                                          uint64_t size, int k,
                                                          int d,
                                                          bool reverseIncluded)
{
    int freqArraySize = 1 << (2 * k);
    int* fa = freqTable(k);
    computingFrequenciesWithMismatches(text, size, k, d, fa);
    int maxLoc = 0;
    vector<string> r;
    if (!reverseIncluded) {
        for (int i = 0; i < freqArraySize; i++) {
            int f = fa[i];
            if (f > maxLoc)
                maxLoc = f;
        }
        for (int i = 0; i < freqArraySize; i++) {
            if (maxLoc == fa[i]) {
                r.push_back(numToPattern(i, k));
            }
        }
    } else {
        int* nfa = freqTable(k);
        bool* processed = new bool[freqArraySize];
        memset(processed, 0, sizeof(bool) * freqArraySize);
        for (int i = 0; i < freqArraySize; i++) {
            if (!processed[i]) {
                int g = fa[i];
                int reverseLoc = patternToNumber(
                    reverseComplement(numToPattern(i, k)).c_str(), k);
                int h = fa[reverseLoc];
                nfa[i] = g + h;
                nfa[reverseLoc] = g + h;
                processed[i] = true;
                processed[reverseLoc] = true;

                if (g + h > maxLoc)
                    maxLoc = g + h;
            }
        }

        for (int i = 0; i < freqArraySize; i++) {
            if (nfa[i] == maxLoc) {
                r.push_back(numToPattern(i, k));
            }
        }

        delete[] nfa;
        delete[] processed;
    }
    delete[] fa;

    return make_pair(r, maxLoc);
}

void readLineFedInput(string& input, istream& stream)
{
    string s;
    while (getline(stream, s)) {
        for (char c : s) {
            if (c != '\n' && c != '\r')
                input += c;
        }
    }
}

void validateInput(const string& s)
{
    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        switch (c) {
        case 'A':
        case 'C':
        case 'G':
        case 'T':
            break;
        default:
            cout << "Invalid character: " << c << " at: " << i << endl;
            while (1)
                ;
        }
    }
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
        int kmerCount = (0x01 << (k * 2));
        int* clump = new int[kmerCount];
        memset(clump, 0, sizeof(int) * kmerCount);
        const char* text = _genome.c_str();
        for (int i = 0; i < _genome.size() - L + 1; i++) {
            cout << i << "\n";
            int* a = new int[kmerCount];
            memset(a, 0, sizeof(int) * kmerCount);
            for (int i = 0; i < L - k + 1; i++) {
                int loc = patternToNumber(text + i, k);
                // cout << loc << " ";
                a[loc]++;
            }

            for (int i = 0; i < kmerCount; i++) {
                if (a[i] >= t) {
                    clump[i] = 1;
                }
            }
            delete[] a;
        }

        vector<string> res;
        for (int i = 0; i < kmerCount; i++) {
            if (clump[i] == 1) {
                string p = numToPattern(i, k);
                res.push_back(p);
            }
        }
        delete[] clump;

        return res;
    }
    vector<string> clumps(int k, int L, int t, uint64_t searchwindowSize = 0,
                          uint64_t startpos = 0)
    {
        if (searchwindowSize == 0)
            searchwindowSize = _genome.size();

        int kmerCount = (0x01 << (k * 2));
        char* clump = new char[kmerCount];
        memset(clump, 0, sizeof(char) * kmerCount);
        uint64_t end = searchwindowSize;
        const char* text = _genome.c_str() + startpos;

        if (startpos + searchwindowSize > _genome.size())
            searchwindowSize = _genome.size() - startpos;

        int* temp = new int[kmerCount];
        memset(temp, 0, sizeof(int) * kmerCount);
        int prevLoc = patternToNumber(text, k);
        int backLoc = prevLoc;
        temp[prevLoc]++;
        if (temp[prevLoc] >= t)
            clump[prevLoc] = 1;

        for (int i = 1; i < L - k + 1; i++) {
            int loc = fastNextPos(prevLoc, k, _nucleoLookup[text[i + k - 1]]);
            temp[loc]++;
            if (temp[loc] >= t)
                clump[loc] = 1;
            prevLoc = loc;
        }

        for (int i = 1; i < end - L + 1; i++) {
            temp[backLoc]--;
            backLoc = fastNextPos(backLoc, k, _nucleoLookup[text[i + k - 1]]);
            prevLoc = fastNextPos(prevLoc, k, _nucleoLookup[text[i + L - 1]]);
            temp[prevLoc]++;
            if (temp[prevLoc] >= t)
                clump[prevLoc] = 1;
        }

        delete[] temp;

        vector<string> res;
        for (int i = 0; i < kmerCount; i++) {
            if (clump[i] == 1) {
                string p = numToPattern(i, k);
                res.push_back(p);
            }
        }
        delete[] clump;

        return res;
    }

    int skew(uint64_t i, uint64_t s = 0)
    {
        if (s + i > _genome.size())
            return -1;

        int diff = 0;
        uint64_t end = s + i;
        for (uint64_t p = s; p < end; p++) {
            diff = _skew(diff, p);
        }

        return diff;
    }

    vector<int> minimumSkew() const
    {
        vector<int> res;
        int diff = 0;
        int min = diff;

        for (uint64_t i = 0; i < _genome.size(); i++) {
            diff = _skew(diff, i);

            if (diff < min) {
                min = diff;
                res.clear();
                res.push_back(i + 1);
            } else if (diff == min)
                res.push_back(i + 1);
            // cout << diff << "\n";
        }

        return res;
    }

    vector<string> findDnaBoxCandidates(int k, int d, int windowSizeBefore,
                                        int windowSizeAfter)
    {
        vector<int> skews = minimumSkew();
        // TODO check if argument does not fall out of memory range
        auto res = findMostFreqKmersWithMismatches(
            _genome.c_str() + skews[0] - windowSizeBefore,
            windowSizeBefore + windowSizeAfter, k, d, true);
        return res.first;
    }

    size_t size() const
    {
        return _genome.size();
    }

private:
    inline int _skew(int before, uint64_t pos) const
    {
        if (_genome[pos] == 'G')
            before++;
        else if (_genome[pos] == 'C')
            before--;
        return before;
    }

private:
    const string& _genome;
    char _nucleoLookup[256];
};

unordered_set<string> commonSet(const vector<vector<string>> candidateSets)
{
    unordered_set<string> common;
    for (int i = 0; i < candidateSets.size(); i++) {
        const vector<string>& se = candidateSets[i];
        for (const string& s : se) {
            bool inAll = true;
            for (int j = i + 1; j < candidateSets.size(); j++) {
                const vector<string>& hh = candidateSets[j];
                if (find(hh.begin(), hh.end(), s) == hh.end()) {
                    inAll = false;
                    break;
                }
            }
            if ((inAll && i != candidateSets.size() - 1) ||
                (inAll && candidateSets.size() == 1))
                common.insert(s);
        }
    }
    return common;
}

const char* findPattern(const char* where, size_t whereLen, const char* what,
                        size_t whatLen)
{
    const char* end = where + whereLen;
    const char* curr = where;
    while (curr + whatLen - 1 < end) {
        if (!memcmp(what, curr, whatLen)) {
            return curr;
        }
        curr++;
    }

    return NULL;
}

class Motif
{
public:
    template <class T>
    struct nucleofreq
    {
        nucleofreq()
        {
            memset(t, 0, sizeof(T) * 4);
        }
        T t[4];
    };

    /*
     * complexity:
     * O(numberOfDNAs * dnaLength * numOfPatternSimilar * numOfPatternSimilar *
     * numberOfDNAs)
     */
    static unordered_set<string> motifEnumeration(vector<string> dnaSet, int k,
                                                  int d)
    {
        unordered_set<string> solution;

        for (const string& dna : dnaSet) {
            for (int i = 0; i <= dna.size() - k + 1; i++) {
                vector<string> patterns =
                    generateSimilar(dna.c_str() + i, k, d);
                for (const string& p : patterns) {
                    vector<string> pvars = generateSimilar(p.c_str(), k, d);
                    bool present = true;
                    for (const string& d : dnaSet) {
                        bool pp = false;
                        for (const string& pvar : pvars) {
                            if (findPattern(d.c_str(), d.size(), pvar.c_str(),
                                            pvar.size())) {
                                pp = true;
                                break;
                            }
                        }
                        if (!pp) {
                            present = false;
                        }
                    }
                    if (present) {
                        solution.insert(p);
                    }
                }
            }
        }

        return solution;
    }
    static double entropy(const vector<double>& probs)
    {
        double acc = 0;
        for (double p : probs) {
            if (p != 0.0) {
                acc += p * log2l(p);
            }
        }
        return acc;
    }

    static vector<nucleofreq<int>> scoreMatrix(const vector<string>& matrix)
    {
        char nucleoLookup[256];
        nucleoLookup['A'] = 0;
        nucleoLookup['C'] = 1;
        nucleoLookup['G'] = 2;
        nucleoLookup['T'] = 3;
        int len = matrix[0].size();
        vector<nucleofreq<int>> freqs(len);
        for (int c = 0; c < len; c++) {
            nucleofreq<int> nf;
            for (const string& s : matrix) {
                nf.t[nucleoLookup[s[c]]]++;
            }
            freqs[c] = nf;
        }
        return freqs;
    }

    static vector<double> entropyMatrix(const vector<string>& matrix)
    {
        vector<nucleofreq<int>> counts = scoreMatrix(matrix);

        vector<double> entropies;
        for (int i = 0; i < matrix.size(); i++) {
            vector<double> probs;
            for (int j = 0; j < 4; j++) {
                probs.push_back(counts[i].t[j] / (double)matrix.size());
            }
            entropies.push_back(entropy(probs));
        }

        return entropies;
    }

    static string consensus(const vector<string>& matrix)
    {
        static char nucleosLookup[] = {'A', 'C', 'G', 'T'};
        vector<nucleofreq<int>> counts = scoreMatrix(matrix);
        string consensus = "";
        for (int i = 0; i < matrix.size(); i++) {
            int maxidx = 0;
            int max = 0;
            for (int j = 0; j < 4; j++) {
                if (counts[i].t[j] > max) {
                    max = counts[i].t[j];
                    maxidx = j;
                }
            }
            consensus.push_back(nucleosLookup[maxidx]);
        }

        return consensus;
    }

    // static vector<string> bruteForceMotifSearch(vector<string> dna)
    // {
    // 	for(const string& s : dna)
    // 	{

    // 	}
    // }

    static int distance(const string& pattern, const string& text)
    {
        int k = pattern.size();
        int min = 0;
        for (int i = 0; i <= text.size() - k; i++) {
            // cout << "Iter: " << i << endl;
            int d = hammingDistance(pattern.c_str(), text.c_str() + i, k);
            if (d < min || i == 0) {
                min = d;
            }
        }
        return min;
    }

    static int distance(const string& pattern, const vector<string>& dna)
    {
        int sum = 0;
        for (const string& t : dna) {
            sum += distance(pattern, t);
        }
        return sum;
    }

    static string medianString(const vector<string>& dna, int k)
    {
        string candidate = "";
        int min = 0;
        for (int i = 0; i < (1 << 2 * k); i++) {
            string pattern = numToPattern(i, k);
            int c = distance(pattern, dna);
            if (c < min || i == 0) {
                min = c;
                candidate = pattern;
            }
        }
        return candidate;
    }
};

vector<string> readfile(const string& filename)
{
    vector<string> res;
    ifstream f(filename);
    if (f.is_open()) {
        string buf;
        while (getline(f, buf)) {
            res.push_back(buf);
        }
    }
    return res;
}

int Count(string& text, string& pattern)
{
    size_t textlen = text.size();
    size_t patternlen = pattern.size();
    int c = 0;
    for (int i = 0; i < textlen - patternlen + 1; i++) {
        if (string(text.begin() + i, text.begin() + i + patternlen) ==
            pattern) {
            c++;
        }
    }
    return c;
}

/*
****************************
    Scratchpad
*****************************
*/

int main()
{
    string text;
    string pattern;
    cin >> text;
    cin >> pattern;

    cout << patternCount(text, pattern.c_str(), pattern.size());
}

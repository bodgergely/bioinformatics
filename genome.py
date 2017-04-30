
"""

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

"""

import math
import time

def offset(c, pos):
    alphabet = ['A', 'C', 'G', 'T']
    i=0;
    
    v = 0
    for i in range(4):
        if c == alphabet[i]:
            v = i
            break

    offs = v * int(math.pow(4, pos))
    return offs;


def patternToNumber(pattern):
    loc = 0
    k = len(pattern)
    for i in range(k):
        loc+=offset(pattern[i], k - i - 1)

    return loc;


def numToPattern(pos, k):
    alphabet = ['A', 'C', 'G', 'T']
    f = pos
    s = ""
    for i in range(k-1, -1, -1):
        t = int(math.pow(4, i))
        l = f/t
        s+=alphabet[l]
        f = f % t;
    return s;

class Genome:
    def __init__(self, genome):
        self._genome = genome
    def freqArraySorted(self, k):
        text = self._genome
        coded = []
        for i in range(len(text) - k + 1):
            pattern = text[i:i+k]
            loc = patternToNumber(pattern)
            coded.append(loc)
        coded.sort()
        maxLocs = []
        prev = coded[0]
        maxc = 1
        counter = 1
        currLoc = 0
        for i in range(1, len(coded)):
            if coded[i]!=prev:
                if counter > maxc:
                    maxc = counter
                    maxLocs = []
                    maxLocs.append(coded[currLoc])
                elif counter == maxc:
                    maxLocs.append(coded[currLoc])
                currLoc = i
                counter = 1
                prev = coded[i]
            else:
                counter+=1
        
        res = []
        for l in maxLocs:
            res.append(numToPattern(l, k))
    
        return res
        
"""
import sys

file = sys.argv[1]
f = open(file, 'r')
text = f.read()
genome = Genome(text)
start = time.time()
kmers = genome.freqArraySorted(9)
end = time.time()
print "Took secs: " + str((end - start))
for k in kmers:
    print k   
        
"""     
        
        
        
        

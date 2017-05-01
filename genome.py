

import math
import time
from matplotlib import pyplot as plt
import sys

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
        
        
        
def plot(rows):
    plt.plot(rows)
    plt.show()


def readFile(file):
    f = open(file, 'r')
    rows = []
    for l in f:
        rows.append(int(l))
    return rows

rows = readFile(sys.argv[1])
plot(rows)

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
        
        
        
        

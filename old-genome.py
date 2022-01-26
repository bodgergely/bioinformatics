#!python

import math
import time
from matplotlib import pyplot as plt
import sys

# from docutils.nodes import row
# from sympy.interactive.tests.test_ipython import readline


def offset(c, pos):
    alphabet = ["A", "C", "G", "T"]
    i = 0

    v = 0
    for i in range(4):
        if c == alphabet[i]:
            v = i
            break

    offs = v * int(math.pow(4, pos))
    return offs


def patternToNumber(pattern):
    loc = 0
    k = len(pattern)
    for i in range(k):
        loc += offset(pattern[i], k - i - 1)

    return loc


def numToPattern(pos, k):
    alphabet = ["A", "C", "G", "T"]
    f = pos
    s = ""
    for i in range(k - 1, -1, -1):
        t = int(math.pow(4, i))
        l = f // t
        s += alphabet[l]
        f = f % t
    return s


class Genome:
    def __init__(self, genome):
        self._genome = genome

    def freqArraySorted(self, k):
        text = self._genome
        coded = []
        for i in range(len(text) - k + 1):
            pattern = text[i : i + k]
            loc = patternToNumber(pattern)
            coded.append(loc)
        coded.sort()
        maxLocs = []
        prev = coded[0]
        maxc = 1
        counter = 1
        currLoc = 0
        for i in range(1, len(coded)):
            if coded[i] != prev:
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
                counter += 1

        res = []
        for l in maxLocs:
            res.append(numToPattern(l, k))

        return res


def plot(rows):
    plt.plot(rows)
    plt.show()


def readSkew(file):
    f = open(file, "r")
    rows = []
    for l in f:
        rows.append(int(l))
    f.close()
    return rows


def readLines(file):
    res = ""
    with open(file, "r") as f:
        for line in f:
            res += line
    return res


def fromTo(row, start, end):
    s = ""
    return row[start:end]


def skewPlot(fileName):
    rows = readSkew(sys.argv[1])
    plot(rows)


def mostFrequentWords(text, k):
    table = {}
    n = len(text)
    for i in range(0, n - k + 1):
        pattern = text[i : i + k]
        if pattern not in table:
            table[pattern] = 1
        else:
            table[pattern] += 1
    maxSeen = 0
    res = []
    for pat, pc in table.items():
        if pc > maxSeen:
            res.clear()
            maxSeen = pc
            res.append(pat)
        elif pc == maxSeen:
            res.append(pat)
        else:
            pass
    return res


inp = [l for l in sys.stdin]
text = inp[0]
k = int(inp[1])

for l in mostFrequentWords(text, k):
    print(l)


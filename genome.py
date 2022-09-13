import fileinput
import sys
import array
from collections import defaultdict
from typing import List, Set, Dict

"""
k: kmer
"""

def pattern_count(text, pattern):
    i = 0
    n = len(pattern)
    count = 0
    if n == 0:
        return count
    end = len(text) - n
    while i <= end:
        if text[i:i+n] == pattern:
            count += 1
        i += 1
    return count


def most_frequent_kmer(text, k):
    res = set()
    n = 0
    if len(text) < k:
        return res, n
    table = defaultdict(int)
    for i in range(0, len(text) - k + 1):
        table[text[i : i + k]] += 1
    sorted_table = [
        (v, k) for k, v in sorted(table.items(), key=lambda item: item[1], reverse=True)
    ]

    if len(sorted_table) == 0:
        return res, n
    n = sorted_table[0][0]
    for el in sorted_table:
        if n == el[0]:
            res.add(el[1])
        else:
            break
    return res, n

def frequency_table(text, k):
    table = defaultdict(int)
    for i in range(0, len(text) - k + 1):
        table[text[i:i + k]] += 1
    return table


def reverse_complement(text, abc_table):
    res = []
    for c in text[::-1]:
        try:
            res.append(abc_table[c])
        except KeyError as e:
            print(f"Could not find {c} as key in abc_table: {abc_table}")
            raise e
    return "".join(res)

GENOME_COMPLEMENT_TABLE = {
    'a':'t',
    't':'a',
    'c':'g',
    'g':'c',
}

def reverse_complement_genome(text):
    return reverse_complement(text, abc_table=GENOME_COMPLEMENT_TABLE)


def pattern_match_indexes(text, pattern) -> List[int]:
    res = []
    i = 0
    n = len(pattern)
    if n == 0:
        return res
    end = len(text) - n
    while i <= end:
        if text[i:i+n] == pattern:
            res.append(i)
        i += 1
    return res


def position_table(text: str, k: int) -> Dict[str, List[int]]:
    pos_map = {}
    for i in range(len(text) - k + 1):
        pattern = text[i:i + k]
        try:
            pos_map[pattern].append(i)
        except KeyError:
            pos_map[pattern] = [i]
    return pos_map

def is_clump(positions: List[int], k: int, L: int, t: int) -> bool:
    for i in range(len(positions) - t + 1):
        if positions[i + t - 1] - positions[i] <= L - k:
            return True
    return False

def find_clumps(text: str, k: int, L: int, t: int) -> Set[str]:
    pos_map = position_table(text, k)
    clumps = set()
    for pattern in pos_map:
        positions = pos_map[pattern]
        if len(positions) >= t:
            if is_clump(positions, k, L, t):
                clumps.add(pattern)
    return clumps


# skew
# skew(genome, i) = Num('G', i) - Num('C', i)

def skew_table(genome):
    table = [0 for _ in range(len(genome)+1)]
    n = len(genome)
    for i in range(0, n):
        val = 0
        if genome[i] == 'g':
            val = 1
        elif genome[i] == 'c':
            val = -1
        table[i+1] = table[i] + val
    return table



def main(args):
    inp = fileinput.input()
    text = inp.readline().strip()
    text = text.lower()
    for c in skew_table(text):
        print(c, end=' ')

if __name__ == "__main__":
    main(sys.argv[1:])



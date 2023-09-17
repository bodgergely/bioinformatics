import fileinput
import sys
import array
from collections import defaultdict
from typing import List, Set, Dict, Iterable, Tuple

"""
k: kmer
"""

GENOME_TABLE = ['A', 'T', 'C', 'G']

GENOME_COMPLEMENT_TABLE = {
    'A':'T',
    'T':'A',
    'C':'G',
    'G':'C',
}

def hamming_distance(p: str, q: str) -> int:
    assert len(p) == len(q)
    return sum([1 for a,b in zip(p, q) if a != b])


def pattern_count_helper(text, pattern, is_equal):
    i = 0
    n = len(pattern)
    count = 0
    if n == 0:
        return count
    end = len(text) - n
    while i <= end:
        if is_equal(text[i:i+n], pattern):
            count += 1
        i += 1
    return count

def pattern_count(text, pattern):
    return pattern_count_helper(text, pattern, lambda x, y: x == y)

def pattern_count_approx(text, pattern, d):
    return pattern_count_helper(text, pattern, 
            lambda x, y: hamming_distance(x, y) <= d)


def most_frequent_kmer(text, k) -> Tuple[Set, int]:
    res = set()
    n = 0
    if len(text) < k:
        return res, n
    table = defaultdict(int)
    for i in range(0, len(text) - k + 1):
        table[text[i : i + k]] += 1

    sorted_table = [
        (v, k) for k, v in sorted(table.items(), 
            key=lambda item: item[1], reverse=True)
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

"""
The set of all patterns those Hamming distance is at most d
"""
def generate_neighbors(pattern, max_dist):
    # print("max_dist:", max_dist, type(max_dist))
    res = {pattern} # the 0 distance is the same as the original
    n = len(pattern)
    def build_from_indexes(s, idxs):
        res = []
        # print("From idxs: ", idxs)
        for i in idxs:
            for g in GENOME_TABLE:
                if g != s[i]:
                    res.append(s[:i] + g + s[i+1:])
        # print("Generated: ", res)
        return set(res)
             
    def foo(indexes, start):
        res.update(build_from_indexes(pattern, indexes))
        if len(indexes) >= max_dist:
            return
        for i in range(start, n):
            foo(indexes+[i], i+1)

    foo([], 0)
    return res



def frequent_words_with_mismatches(text: str, k: int, d: int) -> List[str]:
    patterns = []
    freq_map = defaultdict(int)
    n = len(text)
    for i in range(0, n-k+1):
        pattern = text[i:i+k]
        # print(i, pattern)
        neighborhood = generate_neighbors(pattern, d)
        for neighbor in neighborhood:
            if neighbor == 'GCACACAGAC':
                print('GCACACAGAC is neighbor of: ', pattern, "start: ", i)
            freq_map[neighbor] += 1 
    max_count = max(freq_map.values()) 
    print("max_count:", max_count)
    print(sorted([(k, v) for k, v in freq_map.items() if v >= 8], key=lambda x: x[1]))
    # print(freq_map)
    patterns = [key for key, val in freq_map.items() if val == max_count]
    print('GCACACACAC:', freq_map['GCACACACAC'])
    print('GCACACAGAC:', freq_map['GCACACAGAC'])
    return patterns



def reverse_complement(text, abc_table):
    res = []
    for c in text[::-1]:
        try:
            res.append(abc_table[c])
        except KeyError as e:
            print(f"Could not find {c} as key in abc_table: {abc_table}")
            raise e
    return "".join(res)

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

def pattern_match_indexes_approx(text, pattern, d: int) -> Iterable[int]:
    res = []
    i = 0
    n = len(pattern)
    if n == 0:
        return res
    end = len(text) - n
    while i <= end:
        if hamming_distance(text[i:i+n], pattern) <= d:
            res.append(i)
        i += 1
    return res



def position_table(text: str, k: int) -> Dict[str, List[int]]:
    pos_map = {}
    for i in range(len(text) - k + 1):
        pattern = text[i:i + k]
        pos_map.setdefault(pattern, []).append(i)
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
        if genome[i] == 'G':
            val = 1
        elif genome[i] == 'C':
            val = -1
        table[i+1] = table[i] + val
    return table

def minimum_skew(genome):
    table = skew_table(genome)
    min_val = sys.maxsize
    for i, val in enumerate(table):
        if val < min_val:
            min_val = val
    min_idxs = []
    for i, val in enumerate(table):
        if val == min_val:
            min_idxs.append(i)
    return min_idxs



def main(args):
    inp = fileinput.input()
    text = inp.readline().strip().upper()
    k, d = [int(x) for x in inp.readline().split(' ')]
    # for c in skew_table(text):
        # print(c, end=' ')
    # gen_test(args[0], int(args[1]))
    words = frequent_words_with_mismatches(text, k, d)
    for n in words:
        print(n, end=' ')
    print("")


    # debug
    s = 'GCACACAGAC'
    print("Neighbors of ", s, " are these:")
    neighs = generate_neighbors(s, 1)
    for n in neighs:
        locs = pattern_match_indexes(text, n)
        if len(locs) > 0:
            print("String: ", n)
            for i in locs:
                print("found at:", i)
    # debug end

if __name__ == "__main__":
    main(sys.argv[1:])



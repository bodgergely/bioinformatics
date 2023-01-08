import pytest
import genome
from collections import defaultdict

DATA_FOLDER = "./test_data/"

def test_most_frequent_kmer():
    from genome import most_frequent_kmer
    assert most_frequent_kmer("AABCCCDDD", 2) == ({"CC", "DD"}, 2)
    assert most_frequent_kmer("AABCCCDDD", 2) == ({"DD", "CC"}, 2)
    assert most_frequent_kmer("AABCCCDDDEEEE", 2) == ({"EE"}, 3)

def test_pattern_count():
    from genome import pattern_count
    assert pattern_count("AAAA", "A") == 4
    assert pattern_count("AAAA", "AAAA") == 1
    assert pattern_count("AAAA", "AAAAA") == 0
    assert pattern_count("", "") == 0
    assert pattern_count("S", "") == 0
    assert pattern_count("SS", "") == 0

def test_reverse_complement_genome():
    from genome import reverse_complement_genome
    assert reverse_complement_genome("ATCG") == "CGAT"
    assert reverse_complement_genome("") == ""
    with pytest.raises(KeyError):
        reverse_complement_genome("AF") == "AT"
    with pytest.raises(KeyError):
        reverse_complement_genome("a")
        reverse_complement_genome("t")
        reverse_complement_genome("c")
        reverse_complement_genome("g")
def test_pattern_match_indexes():
    from genome import pattern_match_indexes
    assert pattern_match_indexes("AAAA", "A") == [0,1,2,3]
    assert pattern_match_indexes("ABCD", "FT") == [] 
    assert pattern_match_indexes("A", "") == []
    assert pattern_match_indexes("AA", "") == []
    assert pattern_match_indexes("", "") == []

def test_frequency_table():
    from genome import frequency_table
    assert frequency_table("AABCDDABC", 2) == {"AA": 1, "DD": 1, "AB":2, "BC": 2, "CD": 1, "DA": 1}

def test_find_clumps():
    from genome import find_clumps
    text, nums = [line.strip() for line in open(DATA_FOLDER + "test_find_clumps-input.txt")]
    k, L, t = [int(v) for v in nums.split()]
    text = text.upper()
    res = find_clumps(text, k, L, t)
    with open(DATA_FOLDER + "test_find_clumps-output.txt") as f:
        assert res == set([v for v in f.readline().strip().split()])

def test_minimum_skew():
    from genome import minimum_skew
    genome = "TAAAGACTGCCGAGAGGCCAACACGAGTGCTAGAACGAGGGGCGTAAACGCGGGTCCGAT"
    expected = {11, 24}
    assert set(minimum_skew(genome)) == expected

def test_hamming_distance():
    from genome import hamming_distance
    assert hamming_distance("hello", "bello") == 1
    with pytest.raises(Exception):
        assert hamming_distance("hello", "belloo") == 1




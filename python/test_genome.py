import pytest
import genome
from collections import defaultdict

DATA_FOLDER = "./test_data/"

def test_most_frequent_kmer():
    from genome import most_frequent_kmer
    assert most_frequent_kmer("aabcccddd", 2) == ({"cc", "dd"}, 2)
    assert most_frequent_kmer("aabcccddd", 2) == ({"dd", "cc"}, 2)
    assert most_frequent_kmer("aabcccdddeeee", 2) == ({"ee"}, 3)

def test_pattern_count():
    from genome import pattern_count
    assert pattern_count("aaaa", "a") == 4
    assert pattern_count("aaaa", "aaaa") == 1
    assert pattern_count("aaaa", "aaaaa") == 0
    assert pattern_count("", "") == 0
    assert pattern_count("s", "") == 0
    assert pattern_count("ss", "") == 0

def test_reverse_complement_genome():
    from genome import reverse_complement_genome
    assert reverse_complement_genome("atcg") == "cgat"
    assert reverse_complement_genome("") == ""
    with pytest.raises(KeyError):
        reverse_complement_genome("af") == "at"
    with pytest.raises(KeyError):
        reverse_complement_genome("A")
        reverse_complement_genome("T")
        reverse_complement_genome("C")
        reverse_complement_genome("G")
def test_pattern_match_indexes():
    from genome import pattern_match_indexes
    assert pattern_match_indexes("aaaa", "a") == [0,1,2,3]
    assert pattern_match_indexes("abcd", "ft") == [] 
    assert pattern_match_indexes("a", "") == []
    assert pattern_match_indexes("aa", "") == []
    assert pattern_match_indexes("", "") == []

def test_frequency_table():
    from genome import frequency_table
    assert frequency_table("aabcddabc", 2) == {"aa": 1, "dd": 1, "ab":2, "bc": 2, "cd": 1, "da": 1}

def test_find_clumps():
    from genome import find_clumps
    text, nums = [line.strip() for line in open(DATA_FOLDER + "test_find_clumps-input.txt")]
    k, L, t = [int(v) for v in nums.split()]
    res = find_clumps(text, k, L, t)
    with open(DATA_FOLDER + "test_find_clumps-output.txt") as f:
        assert res == set([v for v in f.readline().strip().split()])

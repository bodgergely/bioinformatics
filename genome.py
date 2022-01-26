import fileinput
from collections import defaultdict

lines = [l.strip() for l in fileinput.input()]


genome = lines[0]

"""
k: kmer
"""


def most_frequent(text, k):
    res = []
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
            res.append(el[1])
        else:
            break
    return res, n


print(most_frequent(genome, 9))


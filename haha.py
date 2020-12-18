
with open("big.txt", "w") as f:
    for i in range(0, 1000000):
        f.write(str(i))
        f.write("\n")
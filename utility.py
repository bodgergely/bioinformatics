from __future__ import print_function
import sys
import random

def main():
    for i in range(int(sys.argv[1])):
        sys.stdout.write(chr(random.randrange(ord('a'), ord('z')+1)))

if __name__ == "__main__":
    main()
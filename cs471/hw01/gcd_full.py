#! /usr/bin/env python3
import sys

def gcdI(i, j):
    while i not j:
        if (i > j):
            i = i - j
        else:
            j = j - i
    return i

def gcdR(i, j):
    return j if (i % j == 0) else gcdR(j, i % j)

def main():
    if len(sys.argv) != 3:
        print('Pass two integer arguments')
        exit()

    i, j = int(sys.argv[1]), int(sys.argv[2])
    print('gcdI({}, {}) = {}'.format(i, j, gcdI(i,j)))
    print('gcdR({}, {}) = {}'.format(i, j, gcdR(i,j)))

if __name__ == '__main__':
    main()

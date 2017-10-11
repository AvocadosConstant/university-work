#!/usr/bin/env python3
"""
    This script does the following: TODO

    Developed for the Fall 2017 section of CS 436 - Machine Learning
    at Binghamton University taught by Professor Arti Ramesh.
"""

import os
import string
import argparse
import pandas as pd

def gen_words(filename):
    remove = dict.fromkeys(map(ord, '\n' + string.punctuation))
    with open(filename, 'rb+') as f:
        for line in f:
            for word in str(line).lower().translate(remove).split():
                yield word

def main():
    parser = argparse.ArgumentParser(
        description='Classify some text as spam or ham using a multinomial Naive Bayes classifier.')

    parser.add_argument('stop_dir',     help='Directory containing stopWords.txt')
    parser.add_argument('train_dir',    help='Directory containing training data')
    parser.add_argument('test_dir',     help='Directory containing testing data')

    args = vars(parser.parse_args())

    for directory in [args['train_dir'] + spamham for spamham in ['spam/', 'ham/']]:
        for filename in os.listdir(directory):
            #print(directory + filename)
            [print(word) for word in gen_words(directory+filename)]


if __name__ == '__main__':
    main()

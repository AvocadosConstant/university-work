#!/usr/bin/env python3
"""
    This script does the following: TODO

    Developed for the Fall 2017 section of CS 436 - Machine Learning
    at Binghamton University taught by Professor Arti Ramesh.
"""

import os
import string
import argparse
from classytext import NaiveBayes

def train(model, path):
    labels_paths = [
        (label, path + label + '/')
        for label in ['spam', 'ham']]
    model.train(labels_paths)

def main():
    parser = argparse.ArgumentParser(
        description='Classify some text as spam or ham'\
            ' using a multinomial Naive Bayes classifier.')

    parser.add_argument('stop_dir',  help='Directory containing stopWords.txt')
    parser.add_argument('train_dir', help='Directory containing training data')
    parser.add_argument('test_dir',  help='Directory containing testing data')

    args = vars(parser.parse_args())

    model = NaiveBayes()
    train(model, args['train_dir'])


if __name__ == '__main__':
    main()

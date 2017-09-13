#!/usr/bin/env python3
"""
    This script builds, tests, prunes, and analyzes
    a decision tree built from a provided dataset.

    Developed for the Fall 2017 section of CS 436 - Machine Learning
    at Binghamton University taught by Professor Arti Ramesh.
"""

import argparse

def main():
    parser = argparse.ArgumentParser(
        description='Classify some data using decision trees.')

    parser.add_argument('training-set',   help='csv training set')
    parser.add_argument('validation-set', help='csv validation set')
    parser.add_argument('test-set',       help='csv test set')
    parser.add_argument('to-print',       help='prints tree', choices=['yes', 'no'])
    parser.add_argument('prune',          help='prunes tree', choices=['yes', 'no'])

    args = parser.parse_args()
    print(args)

if __name__ == '__main__':
    main()

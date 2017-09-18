#!/usr/bin/env python3
"""
    This script builds, tests, prunes, and analyzes
    a decision tree built from a provided dataset.

    Developed for the Fall 2017 section of CS 436 - Machine Learning
    at Binghamton University taught by Professor Arti Ramesh.
"""

import argparse
import pandas as pd

import decisiontree

def main():
    parser = argparse.ArgumentParser(
        description='Classify some data using decision trees.')

    parser.add_argument('training-set',   help='csv training set')
    parser.add_argument('validation-set', help='csv validation set')
    parser.add_argument('test-set',       help='csv test set')
    parser.add_argument('to-print',       help='prints tree', choices=['yes', 'no'])
    parser.add_argument('prune',          help='prunes tree', choices=['yes', 'no'])

    args = vars(parser.parse_args())

    training_set = pd.read_csv(args['training-set'])
    validation_set = pd.read_csv(args['validation-set'])
    test_set = pd.read_csv(args['test-set'])

    for heuristic in ['entropy', 'variance_impurity']:
        print('\n--------------------------')
        tree = decisiontree.fit(
                training_set,
                'Class',
                heuristic=heuristic)

        if args['to-print'] == 'yes':
            print(tree)

        predictions = decisiontree.predict(tree, test_set, 'Class')

        print('Accuracy:', decisiontree.measure_accuracy(test_set['Class'], predictions))
    print('\n--------------------------')

    """
    dtree = DT('wesley')
    dtree['0'] = DT('honor')
    dtree['0']['0'] = DT('barclay')
    dtree['0']['0']['0'] = '1'
    dtree['0']['0']['1'] = '0'
    dtree['0']['1'] = DT('tea')
    dtree['0']['1']['0'] = '0'
    dtree['0']['1']['1'] = '1'
    dtree['1'] = '0'
    print(dtree, '\n')

    dtree = DT('Outlook')
    dtree['Sunny'] = DT('Humidity')
    dtree['Sunny']['High'] = 'No'
    dtree['Sunny']['Normal'] = 'Yes'
    dtree['Overcast'] = 'Yes'
    dtree['Rain'] = DT('Wind')
    dtree['Rain']['Strong'] = 'No'
    dtree['Rain']['Weak'] = 'Yes'
    print(dtree)
    """

if __name__ == '__main__':
    main()

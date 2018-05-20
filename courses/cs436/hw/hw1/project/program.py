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

    class_label = 'Class'

    # Train and predict on both heuristics
    for heuristic in ['entropy', 'variance_impurity']:
        print('\n--------------------------')

        tree = decisiontree.fit(
                training_set,
                class_label,
                heuristic=heuristic,
                print_details=True)

        # measure accuracy on test set
        orig_accuracy = decisiontree.measure_accuracy(
            tree, test_set, class_label)

        if args['prune'] == 'yes':
            print('Accuracy before pruning: {}%'.format(
                round(100 * orig_accuracy, 2)))

            # prune on validation set
            decisiontree.reduced_error_pruning(tree, validation_set, class_label)

            # measure pruned accuracy and display improvement
            pruned_accuracy = decisiontree.measure_accuracy(
                tree, test_set, class_label)
            print('Accuracy after pruning:  {}%'.format(
                round(100 * pruned_accuracy, 2)))

            print('\nThere was a {}% improvement in accuracy after pruning.'.format(
                round((100 * (pruned_accuracy - orig_accuracy) / orig_accuracy), 2)))
        else:
            print('Accuracy: {}%'.format(round(100 * orig_accuracy, 2)))

        if args['to-print'] == 'yes':
            print(tree)


    print('\n--------------------------')

if __name__ == '__main__':
    main()

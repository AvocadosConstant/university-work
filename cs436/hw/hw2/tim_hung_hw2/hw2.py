#!/usr/bin/env python3
"""
    This script does the following: TODO

    Developed for the Fall 2017 section of CS 436 - Machine Learning
    at Binghamton University taught by Professor Arti Ramesh.
"""

import os, string, argparse
from classytext import NaiveBayes, BagOfWords, Document

def train(model, path):
    labels_paths = [
        (label, path + label + '/')
        for label in ['spam', 'ham']]
    model.train(labels_paths)

def classify_accuracy(model, path):
    num_docs, num_correct = 0, 0
    labels_paths = [
        (label, path + label + '/')
        for label in ['spam', 'ham']]
    for label, path in labels_paths:
        for filename in os.listdir(path):
            bag = BagOfWords(label)
            doc = Document(path + filename)
            bag.parse(doc)
            if label == model.classify(bag, k=1):
                num_correct += 1
            num_docs += 1
    return num_correct / num_docs

def main():
    parser = argparse.ArgumentParser(
        description='Classify some text as spam or ham'\
            ' using a multinomial Naive Bayes classifier.')
    parser.add_argument('stop_dir',  help='Directory containing stopWords.txt')
    parser.add_argument('train_dir', help='Directory containing training data')
    parser.add_argument('test_dir',  help='Directory containing testing data')
    args = vars(parser.parse_args())

    model = NaiveBayes()
    model.set_stop_words(args['stop_dir'] + 'stopWords.txt')

    train(model, args['train_dir'])

    model.sitrep()

    print('Model Accuracy:',
          classify_accuracy(model, args['test_dir']))

if __name__ == '__main__':
    main()

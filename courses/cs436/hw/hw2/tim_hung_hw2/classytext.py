import os, string, re
from math import log
from collections import Counter


class Document:
    def __init__(self, filename):
        self.filename = filename
        with open(self.filename, 'rb+') as f:
            # extract all words
            self.words = re.findall(r'\w[a-z]+',
                f.read().decode('latin-1').lower())


class BagOfWords:
    def __init__(self, label):
        self.freqs = Counter()
        self.num_docs = 0
        self.word_count = 0
    
    def __len__(self):
        return len(self.freqs)

    def parse(self, doc):
        self.freqs.update(doc.words)
        self.num_docs += 1
        self.word_count = sum(self.freqs.values())

    def remove_words(self, words):
        for word in words:
            if word in self.freqs:
                del self.freqs[word]
        self.word_count = sum(self.freqs.values())


class NaiveBayes:
    def __init__(self):
        self.bags = {}
        self.vocab = Counter()
        self.num_docs = 0
        self.stop_words = []

    def sitrep(self):
        """ Displays some helpful information about the model for testing """
        print('\n============ Model information ============\n')
        print(len(self.vocab), 'unique words')
        for label, bag in self.bags.items():
            print('\n--------\n')
            print('For class label', label)
            print('Trained on ', self.bags[label].num_docs, 'documents')
            print(len(self.bags[label]), 'unique words')
            print('10 most common words:',
                  ', '.join([tup[0] for tup in bag.freqs.most_common(10)]))
        print('\n===========================================\n')

    def set_stop_words(self, path):
        with open(path, 'rb+') as f:
            self.stop_words = f.read().decode('latin-1').split()

    def train(self, labels_paths):
        """ Trains model upon training data """
        for label, path in labels_paths:
            self.bags[label] = BagOfWords(label)
            for filename in os.listdir(path):
                doc = Document(path + filename)
                self.bags[label].parse(doc)

            # remove stop words
            self.bags[label].remove_words(self.stop_words)

            # update vocab
            self.vocab += self.bags[label].freqs

            # update num_docs
            self.num_docs += self.bags[label].num_docs

    def classify(self, test_bag, k=1):
        """ Returns the most probable class given a dataset

        test_bag    BagOfWords of document to classify
        k           Laplace smoothing parameter
        """
        best_class, best_posterior = '', -2**30
        for label, bag in self.bags.items():
            prior = log(bag.num_docs / self.num_docs)
            denominator = bag.word_count + k * len(self.vocab)
            likelihood = 0
            for word, count in test_bag.freqs.items():
                if word not in self.stop_words:
                    numerator = bag.freqs.get(word, 0) + k
                    likelihood += count * log(numerator / denominator)
            posterior = prior + likelihood
            if posterior > best_posterior:
                best_posterior = posterior
                best_class = label
        return best_class

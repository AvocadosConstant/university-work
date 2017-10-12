import os, string

class Document:
    def __init__(self, filename):
        self.filename = filename
        self.remove = dict.fromkeys(
                map(ord, '\n' + string.punctuation))

    def gen_words(self):
        with open(self.filename, 'rb+') as f:
            for line in f:
                #print(str(line).strip().lower().split())
                for word in str(line).strip().lower().translate(
                    self.remove).split():
                    yield word

class BagOfWords:
    def __init__(self):
        self.freqs = {}
        self.size = 0
        self.num_docs = 0
    
    def parse(self, doc):
        for word in doc.gen_words():
            self.freqs[word] = self.freqs.get(word, 0) + 1
        self.size = len(self.freqs)
        self.num_docs += 1

class NaiveBayes:
    def __init__(self):
        self.bags = {}

    def train(self, labels_paths):
        for label, path in labels_paths:
            self.bags[label] = BagOfWords()

            for filename in os.listdir(path):
                doc = Document(path + filename)
                self.bags[label].parse(doc)

            print(label)
            print(self.bags[label].num_docs, 'documents')
            print(self.bags[label].size, 'unique words')

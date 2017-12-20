#!/usr/bin/env python3

import os, argparse, re
from nltk.stem.porter import PorterStemmer

def main():
    parser = argparse.ArgumentParser(
        description='Stems files.')
    parser.add_argument('source_dir',  help='Directory containing source files')
    parser.add_argument('out_dir', help='Directory to output stemmed files')
    args = vars(parser.parse_args())

    stemmer = PorterStemmer()
    for label in ['spam/', 'ham/']:
        for filename in os.listdir(args['source_dir'] + label):
            path = label + filename
            with open(args['source_dir'] + path, 'rb+') as f:
                words = re.findall(r'\w[a-z]+',
                    f.read().decode('latin-1').lower())

                out_filename = args['out_dir'] + path
                os.makedirs(os.path.dirname(out_filename), exist_ok=True)
                with open(out_filename, 'w') as o:
                    o.write(' '.join([stemmer.stem(word) for word in words]))



if __name__ == '__main__':
    main()

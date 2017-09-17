import math
import operator
from functools import reduce

def entropy(probs_list):
    return sum(-p * math.log(p, 2) for p in probs_list if p)

def variance_impurity(probs_list):
    return reduce(operator.mul, probs_list)

def best_attribute(data, class_label, heuristic):
    best_attr = ''
    lowest_impurity = float('inf')
    total_size = len(data)

    # valid attribute generator
    attributes = (attr for attr in data if attr != class_label)

    #print('testing {} candidate attributes'.format(len(data.columns)))
    for attr in attributes:
        weighted_impurity = 0
        #print('testing candidate attribute', attr)

        for value, subset in data.groupby(attr):
            #print('\tvalue {} has a count of {} out of {}'.format(value, len(subset), total_size))
            weight = len(subset) / total_size
            probs_list = [class_count / len(subset)
                         for class_count in subset[class_label].value_counts()]
            if len(probs_list) == 1:
                probs_list.append(0.0)

            #print('\t\tprobs_list', probs_list)

            # use specified information gain heuristic
            if heuristic == 'entropy':
                impurity = entropy(probs_list)
            elif heuristic == 'variance_impurity':
                impurity = variance_impurity(probs_list)
            else:
                raise ValueError('"{}" is not a valid information gain heuristic'.format(heuristic))

            weighted_impurity += weight * impurity

        #print('Attribute {} has a weighted impurity of {}'.format(attr, weighted_impurity))

        # best attribute has lowest impurity
        if weighted_impurity < lowest_impurity:
            lowest_impurity = weighted_impurity
            best_attr = attr
    return best_attr

def fit(data, class_label, heuristic='entropy', print_details=True):

    if print_details:
        print( '\nFitting {} data points on class "{}" with '
               'a decision tree using the {} heuristic...'.format(
                len(data), class_label, heuristic))

    # check for purity
    class_values = data[class_label].unique()
    if len(class_values) == 1:
        # return with pure class value
        return class_values[0]

    # find best attribute to split on
    best_attr = best_attribute(data, class_label, heuristic)

    # build child for each value of best attribute
    node = DecisionTree(best_attr)
    for value, subset in data.groupby(best_attr):
        node[value] = fit(subset.drop(best_attr, axis=1), class_label, heuristic, False)

    # return node with all children
    return node

class DecisionTree:
    def __init__(self, attribute):
        self._attr = attribute
        self._children = {}

    def __getitem__(self, key):
        return self._children[key]

    def __setitem__(self, key, value):
        self._children[key] = value

    def __repr__(self):
        branches = []
        for transition, value in self._children.items():
            sub_val = '\n| '.join(str(value).splitlines())
            branches.append('{} = {} : {}'.format(
                self._attr, transition, sub_val))
        return '\n' + '\n'.join(branches)

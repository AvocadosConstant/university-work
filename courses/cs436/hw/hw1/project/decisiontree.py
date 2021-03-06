import math
import operator
from numpy.random import choice
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
    all_have_same_impurity = True

    #print('\n----------------\ntesting {} candidate attributes'.format(len(data.columns) - 1))
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
        if best_attr and lowest_impurity != weighted_impurity:
            all_have_same_impurity = False
        if weighted_impurity < lowest_impurity:
            lowest_impurity = weighted_impurity
            best_attr = attr
    if all_have_same_impurity:
        return None
    return best_attr


def fit(data, class_label, heuristic='entropy', print_details=False):

    if print_details:
        print( '\nFitting {} data points onto a decision '
                'tree using the {} heuristic...\n'.format(
                len(data), heuristic))

    # check for purity
    class_values = data[class_label].unique()
    if len(class_values) == 1:
        # return with pure class value
        return class_values[0]

    # find best attribute to split on
    best_attr = best_attribute(data, class_label, heuristic)

    freqs = data[class_label].value_counts()

    if best_attr == None:
        # cannot find an attribute to split on => there is noise in the data!
        total_size = len(data)
        return RandomDecision(
            freqs.index.tolist(),
            [count / total_size for count in freqs.tolist()])

    # build child for each value of best attribute
    node = DecisionTree(best_attr, freqs)
    for value, subset in data.groupby(best_attr):
        node[value] = fit(subset.drop(best_attr, axis=1), class_label, heuristic, False)
        if type(node[value]) == DecisionTree:
            node[value]._parent = node
            node[value]._from_value = value

    # return node with all children
    return node


def predict(tree, data, class_label, print_details=False):
    if print_details:
        print( '\nPredicting {} data points with '
               'a decision tree...\n'.format(
                len(data)))

    predictions = []
    # for each datapoint
    for i, datapoint in data.iterrows():
        node = tree
        # traverse down tree until hitting a leaf
        while type(node) == DecisionTree:
            node = node[datapoint[node._attr]]

        # take a random sample if leaf is is a RandomDecision
        if type(node) == RandomDecision:
            predictions.append(node.sample())
        else:
            predictions.append(node)

    return predictions


def prune_node(node):
    # prune and set node to majority vote
    majority_vote = node._class_freqs.index[0]
    node._parent._children[node._from_value] = majority_vote


def restore_pruned_node(node):
    node._parent._children[node._from_value] = node


def reduced_error_pruning(tree, valid_data, class_label):
    print('\nPruning tree...\n')

    # measure original accuracy on validation data
    best_accuracy = measure_accuracy(tree, valid_data, class_label)

    # bfs to visit all nodes
    queue = [tree]
    while queue:
        cur = queue.pop(0)
        for attr_value, child_node in cur._children.items():
            if type(child_node) == DecisionTree:
                queue.append(child_node)

        if cur is not tree:
            prune_node(cur)
            accuracy = measure_accuracy(tree, valid_data, class_label)

            # if pruning the current node doesn't improve accuracy, restore it
            if accuracy > best_accuracy:
                best_accuracy = accuracy
            else:
                restore_pruned_node(cur)


def measure_accuracy(tree, data, class_label):
    correct_classes = data[class_label]
    predicted_classes = predict(tree, data, class_label)

    num_total = len(correct_classes)
    num_match = 0
    for i in range(num_total):
        if correct_classes[i] == predicted_classes[i]:
            num_match += 1
    return num_match / num_total


class DecisionTree:
    def __init__(self, attribute, freqs):
        self._attr = attribute
        self._children = {}
        self._class_freqs = freqs
        self._parent = None
        self._from_value = None

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


class RandomDecision:
    def __init__(self, values, weights):
        if len(values) != len(weights):
            raise ValueError('values and weights must be lists of the same length')
        self._values = values
        self._weights = weights

    def __repr__(self):
        return 'weighted choices: ' + ' '.join(
                [str((self._values[i], self._weights[i]))
                for i in range(len(self._values))])

    def sample(self):
        return choice(self._values, p=self._weights)

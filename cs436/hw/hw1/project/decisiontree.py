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

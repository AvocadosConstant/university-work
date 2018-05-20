#!/usr/bin/env python3

class Tree:
    """A binary tree class."""
    def __init__(self, label, left=None, right=None):
        """
        label -- the value of then node of this tree.
        left -- a Tree to be the left child of this node.
        right -- a Tree to be the right child of this node.

        Names with 2 leading and trailing underscores are system defined
        names that have special definition to the interpreter. (__X__)

        The constructor for a tree
        self.xxx=vvv, creates an instance variable, xxx, with the value, vvv.
        "The first argument (called self by convertion) of a method references 
        the instance object being processed;"

        """

        self.label = label
        self.left = left
        self.right = right

    
    def __repr__(self):
        """If defined, __repr__ is called automatically when class instances
        are printed or converted to strings.

        """
        s = repr(self.label)
        if self.left is not None:
            s = s + " " + repr(self.left)
       
        if self.right is not None:
            s = s + " " + repr(self.right)

        return s

    
    def __iter__(self):
        """A generator for an in-order traversal of the Tree"""
        # All iteration contexts in Python will try the __iter__ method
        # first.  Iteration context works by calling the builtin function
        # iter to find an __iter__ method, which is expected to find an
        # iterator object.  If it is provided, Python repeatedly calls
        # this iterator object's next method to produce the next item
        # until a StopIteration exception is raised."
        if self.left is not None:
            for x in self.left:
                yield x
        
        yield self.label
        if self.right is not None:
            for x in self.right:
                yield x


    def preorder(self):
        """A generator for a pre-order traversal of the Tree"""
        yield self.label

        if self.left is not None:
            for x in self.left:
                yield x
        
        if self.right is not None:
            for x in self.right:
                yield x
   

    def postorder(self):
        """A generator for a post-order traversal of the Tree"""

        if self.left is not None:
            for x in self.left:
                yield x
        
        if self.right is not None:
            for x in self.right:
                yield x
    
        yield self.label


# 1) What order does __iter__() method traverse itself?

    # inorder

# 2) Define a method preorder() that returns a preorder iterator of the tree.
# 3) Define a method postorder() that returns a postorder iterator of the tree.

##  ADD class methods here >>>>
    
########### End of Class Tree ####################
 
#module functions

def tree_factory(seq):
    n = len(seq)
    if n == 0:
        return None
    i = n // 2  # use 1//2 instead of 1/2 to get the truncating behavior
    return Tree(seq[i], tree_factory(seq[:i]), tree_factory(seq[i+1:]))


def print_tree_indented(tree, level=0):
    """Pretty print the levels of a tree."""
    if tree is None:
        return
    if tree.right is not None:
        print_tree_indented(tree.right, level+1)

    print('  '*level + str(level))
    if tree.left is not None:
        print_tree_indented(tree.left, level+1)
    

def tree_generator(n):
    if n <= 0:
        yield None
    else:
        for i in range(n):
            for left in tree_generator(i):
                for right in tree_generator(n - i - 1):
                    yield Tree(0, left, right)



def test():
    """Testing function for the Tree class and associated functions."""
    # Example of Trees
    tree1 = tree_factory([1,2,3,4,5,6,7,8,9])
    print("tree1:")
    print_tree_indented(tree1)
    # expected:
    #    2
    #  1
    #    2
    #      3
    # 0
    #    2
    #  1
    #    2
    #      3

    # A tree of trees
    tree2 = tree_factory([tree_factory([1,2,3,4]),
                          tree_factory(['abc','dfg','stu','xyz'])])
    print("tree2:")
    print_tree_indented(tree2)
    # expected:
    # 0
    #  1

    # printing the items in a tree
    tree = tree_factory("abc")
    # sending an object to the print function automatically invokes the
    # __str__ method, if none is found, it then looks for __repr__
    # This behavior makes an explicit call to repr() or str() much less
    # common

# Remove "#" to uncomment the testing of your program
#    print("in-order.  expecting a b c")
#    for n in tree:
#        print(n)

#    print("post-order.  expecting a c b")
#    for n in tree.postorder():
#        print(n)

#    print("pre-order.  expecting b a c")
#    for n in tree.preorder():
#        print(n)

if __name__ == '__main__':
    # this code will only run when this file is the one being run
    # i.e. it won't run when the module is imported
    # notice these are comments, not a docstring, because this isn't a
    # function
    test()

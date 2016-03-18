
    Tim Hung <thung1@binghamton.edu>
    CS 375 | Programming Assignment 1
    Professor Lei Yu

    --------------------------------------------------------------------------------
    Instructions

        Compilation
            $   javac program#.java
                
                or to compile all programs

            $   javac *.java

            No classpaths to worry about, only one class per program.

        Execution
            $   java program# <filex.txt> <filey.txt> <output.txt>

            Again, no classpaths to worry about.

        Output
            Instructed and expected output will be written to the specified file. 
            Debugging output will be printed to System.err.

    --------------------------------------------------------------------------------
    program1.java

        Data Structures
            Character arrays representing the sequences read in from each file.
            A matrix to memoize the length of the LCS for subproblems in order to prevent wasted repetition.
            A matrix to memoize the direction travelled in filling out the length matrix.

        Time Complexity Analysis
            This program has the most effective algorithm out of the three programs.
            The fillMemos function is O(nm) and the constructLCS function is O(n+m).

    program2.java

        Data Structures
            Character arrays representing the sequences read in from each file.

        Time Complexity Analysis
            This is a poor solution because in its worst case, it runs in O(2^n).
            If the two sequences are completely disjoint, the algorithm will check every single subsequence.

    program3.java

        Data Structures
            Character arrays representing the sequences read in from each file.
            A matrix to memoize the length of the LCS for subproblems in order to prevent wasted repetition.

        Time Complexity Analysis
            This solution improves upon the solution in program 2.

    --------------------------------------------------------------------------------
    Notes

        All three programs are heavily documented with JavaDocs fulfilling some of the descriptive requirements.

        In our lectures, the memoization matrix is taught as being size (n+1) by (m+1) with the first row and column padded with 0s.
        I have chosen a different design, instead with a matrix size (n) by (m) with implied 0s surrounding the matrix.
        In practice, the effect is exactly the same and it allowed for more intuitive arithmetic.


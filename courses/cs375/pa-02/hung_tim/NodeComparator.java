import java.util.Comparator;

/**
 * Used for sorting Nodes in a priority queue
 *
 * Programming Assignment 2 
 * CS 375 - Design and Analysis of Algorithms
 * Professor Lei Yu
 * Spring 2016
 *
 * @author Tim Hung
 */
public class NodeComparator implements Comparator<Node> {
    @Override
    public int compare(Node a, Node b) {
        if(a.bound > b.bound) return -1;
        if(a.bound < b.bound) return 1;
        return 0;
    }
}

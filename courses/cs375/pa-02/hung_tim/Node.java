/**
 * A node in the state space tree of a 0/1 Knapsack Problem representing a current weight, profit, and bound.
 *
 * Programming Assignment 2 
 * CS 375 - Design and Analysis of Algorithms
 * Professor Lei Yu
 * Spring 2016
 *
 * @author Tim Hung
 */
public class Node {
    public Node parent;
    public int level, weight, profit;
    public double bound;

    /**
     * Default constructor
     *
     * @param parent parent of the node
     * @param level level of the node
     * @param weight accumulated weight of the node
     * @param profit accumulated profit of the node
     * @param bound upper bound of profit in children
     */
    public Node() {
        level = 0;
        weight = 0;
        profit = 0;
        bound = 0;
        parent = null;
    }

    /**
     * Normal constructor
     *
     * @param parent parent of the node
     * @param level level of the node
     * @param weight accumulated weight of the node
     * @param profit accumulated profit of the node
     * @param bound upper bound of profit in children
     */
    public Node(Node parent, int level, int weight, int profit, double bound) {
        this.parent = parent;
        this.level = level;
        this.weight = weight;
        this.profit = profit;
        this.bound = bound;
    }

    public String toString() {
        String retString = "Level = " + level;
        retString += "\tWeight = " + weight;
        retString += "\tProfit = " + profit;
        retString += "\tBound = " + bound;
        return retString;
    }
}

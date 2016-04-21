public class Node {
    public Node parent;
    public int level, profit, weight;
    public double bound;

    public Node() {
        level = 0;
        profit = 0;
        weight = 0;
        bound = 0;
        parent = null;
    }

    public Node(Node parent, int level, int profit, int weight, double bound) {
        this.parent = parent;
        this.level = level;
        this.profit = profit;
        this.weight = weight;
        this.bound = bound;
    }

    public String toString() {
        String retString = "Level = " + level;
        retString += "\tProfit = " + profit;
        retString += "\tWeight = " + weight;
        retString += "\tBound = " + bound;
        return retString;
    }
}

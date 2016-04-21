import java.lang.Comparable;

public class Item implements Comparable<Item> {
    
    public int weight, profit;
    public double ratio;

    public Item() {
        this.weight = 0;
        this.profit = 0;
        this.ratio = 0;
    }
    public Item(int weight, int profit) {
        this.weight = weight;
        this.profit = profit;
        this.ratio = 1.0 * profit / weight;
    }

    public int compareTo(Item other) {
        return (int)(other.ratio - this.ratio);
    }   

    public String toString() {
        return "Weight: " + weight + " | Profit " + profit;
    }
}

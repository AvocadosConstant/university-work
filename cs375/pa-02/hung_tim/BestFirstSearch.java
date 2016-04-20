import java.util.*; //LinkedList
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;

public class BestFirstSearch {
    private int[ ] p; //array of sizes
    private int[ ] w; //array of values (both ordered by value density)
    private double W; //knapsack capacity
    private double maxProfit = 0;

    public static void main(String[] args) {
        try (Stream<String> stream = Files.lines(Paths.get(args[0]))) {
            stream.forEach(System.out::println);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

/*
    private Vector bestList; //members of solution set for current best value
    private int numItems; //number of items in set to select from
    //(first item is dummy 0)
    private Queue q;

    public KnapsackBandB (double capacity, double [ ] size, double [ ] value,
            int num) {
        maxValue = 0.0;
        K = capacity;
        s = size;
        v = value;
        numItems = num;
        bestList = null;
        q = new Queue( );
    }
    private void knapsack( ) {
        while (!q.isEmpty( ) ) {
            Node temp = (Node) q.dequeue( );
            if (temp.bound > maxValue) {
                Node u = new Node( );
                u.level = temp.level + 1;
                u.size = temp.size + s[temp.level+1];
                u.value = temp.value + v[temp.level+1];
                u.copyList(temp.contains);
                u.add(temp.level+1);
                if (u.size < K && u.value > maxValue) {
                    maxValue = u.value;
                    bestList = new Vector (u.contains);
                }
                u.bound = bound(u.level, u.size, u.value);
                if (u.bound > maxValue)
                    q.enqueue(u);
                Node w = new Node( );
                w.level = temp.level + 1;
                w.size = temp.size;
                w.value = temp.value;
                w.copyList(temp.contains);
                w.add(temp.level+1);
                w.bound = bound(w.level, w.size, w.value);
                if (w.bound > maxValue)
                    q.enqueue(w);
            }
        }
    }
    private double bound (int item, double size, double value) {
        double bound = value;
        double totalSize = size;
        int k = item + 1;
        if (size > K) return 0.0;
        while (k < numItems && totalSize + s[k] <= K) {
            bound += v[k];
            totalSize += s[k];
            k++;
        }
        if (k < numItems)
            bound += (K - totalSize) * (v[k]/s[k]);
        return bound > maxValue;
    }
    public void findSolution( ) {
        Node root = new Node( );
        root.level = 0;
        root.size = 0.0;
        root.value = 0.0;
        root.bounds = bounds(0, 0.0, 0.0);
        root.copyList(null);
        q.enqueue(root);
        knapsack ( );
        System.out.print("The solution set is: ");
        for (int i = 0; i < bestList.size( ); i++) {
            System.out.print(" " + bestList.get(i) );
            System.out.println( );
            System.out.println("The value contained in the knapsack is: $" +
                    maxValue);
        }
    }
*/
}

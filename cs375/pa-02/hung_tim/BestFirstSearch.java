import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.Scanner;
import java.util.Comparator;
import java.util.PriorityQueue; 
import java.util.Arrays;
import java.util.Stack;

/** 
 * A solution to the 0/1 (Integer) Knapsack problem using a Branch and Bounds Best-first search approach.
 *
 * Programming Assignment 2 
 * CS 375 - Design and Analysis of Algorithms
 * Professor Lei Yu
 * Spring 2016
 *
 * @author Tim Hung
 */
public class BestFirstSearch {

    /**
     * Finds the upper bound profit of a node using the Fractional Knapsack Greedy solution
     *
     * @param items     array of items
     * @param index     first index of item in items[] to be considered
     * @param weight    weight already accumulated from ancestors
     * @param profit    profit already accumulated from ancestors
     * @param capacity  total capacity of the problem
     * @return          upper bound profit of the node == to the optimal fractional knapsack solution
     */
    public static double bound(Item[] items, int index, int weight, int profit, int capacity) {
        double bound = profit;
        int i = index;
        while(weight < capacity && i < items.length) {
            if(weight + items[i].weight <= capacity) {
                weight += items[i].weight;
                bound += items[i].profit;
            } else {
                double fraction = 1.0 * (capacity - weight)/items[i].weight;
                return bound + (fraction * items[i].profit); 
            }
            i++;
        } return bound;
    }

    // I was lazy and did everything in main
    public static void main(String[] args) throws IOException {
        if(args.length < 1) {
            System.err.println("\nMust have at least one argument!");
            System.err.println("\tjava BestFirstSearch <input-file> <output-file>");
            System.err.println("\t<input-file> is required");
            System.err.println("\t<output-file> is optional. If unspecified, prints to standard out.\n");
            System.exit(0);
        }


        ///////////
        // Setup //
        ///////////
        Item[] items = new Item[1];
        int capacity = 0;       // knapsack capacity
        int maxProfit = 0;      // max profit possible with an optimal solution
        int optimalSolSize = 0; // number of items needed for optimal solution
        int visited = 1;        // init at 1 for root node
        int leaves = 1;         // leaves is number of times nodes branches into two children + 1
        int size = 0;           // num of items
        Node finalNode = null;  // node representing the last item added for optimal solution

        /////////////////
        // Parse input //
        /////////////////
        try (Scanner in = new Scanner(new File(args[0]))) {
            String[] vals = in.nextLine().split(",");
            size = Integer.parseInt(vals[0]);
            capacity = Integer.parseInt(vals[1]);
            items = new Item[size];
            System.err.println("\nSize: " +  size + "\tCapacity: " + capacity);
            for(int i = 0; i < size; i++) {
                vals = in.nextLine().split(",");
                items[i] = new Item(Integer.parseInt(vals[0]), Integer.parseInt(vals[1]));
            }
            in.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        Arrays.sort(items); // Sort items in non-ascending order
        System.err.println("\nSorted items:");
        for(Item i : items) {
            System.err.print("\tweight: " + i.weight);
            System.err.print("\tprofit: " + i.profit);
            System.err.println("\tratio: " + i.ratio);
        }

        //////////////////////////////////////////////
        // Begin branch and bound best-first search //
        //////////////////////////////////////////////
        PriorityQueue<Node> prioQ = new PriorityQueue<Node>(new NodeComparator()); 
        // Add root to queue
        prioQ.add(
            new Node(null, -1, 0, 0, bound(
                    items, 
                    0, 
                    0, 
                    0, 
                    capacity
                )
            )
        );
        System.err.println("\nAll nodes visited:\nROOT:\t\t" + prioQ.peek());
        while(prioQ.size() > 0) {
            Node focus = prioQ.poll();  // pop the root into focus
            if(focus.bound > maxProfit) {
                int level = focus.level + 1;    // level of the children
                Item curItem = items[level];    // next item to be added or not added
                leaves++;   // Every time the state space tree branches, a new leaf is created

                // Child node where you DO take the item
                Node yes = new Node(
                    focus,                              // parent node 
                    level,                              // level
                    focus.weight + curItem.weight,      // weight
                    focus.profit + curItem.profit,      // profit
                    bound(                              //bound
                        items, 
                        level+1, 
                        focus.weight + curItem.weight, 
                        focus.profit + curItem.profit, 
                        capacity
                    )
                );
                System.err.println("YES:\t\t" + yes);
                visited++;
                if(yes.weight <= capacity && yes.profit > maxProfit) {
                    maxProfit = yes.profit;
                    optimalSolSize = yes.level;
                    finalNode = yes;    // remember the node representing the optimal solution
                    if(yes.bound > maxProfit) prioQ.add(yes);   // push onto prioQ
                }

                // Child node where you do NOT take the item
                Node no = new Node(
                    focus, 
                    level, 
                    focus.weight, 
                    focus.profit, 
                    bound(
                        items, 
                        level+1, 
                        focus.weight, 
                        focus.profit, 
                        capacity
                    )
                );
                System.err.println("NO:\t\t" + no);
                visited++;
                if(no.bound > maxProfit) prioQ.add(no);
            }
        }

        System.err.println("\nMax profit: " + maxProfit);
        System.err.println("Optimal Solution Size: " + optimalSolSize);
        System.err.println("Visited: " + visited);
        System.err.println("Leaves: " + leaves);

        // use stack to reverse order of route to optimal solution
        Stack<Item> optimalItems = new Stack<Item>();
        System.err.println("Final Node: " + finalNode);
        Node tmp = finalNode;
        while(tmp.parent != null) { // starting from the optimal node
            Node parent = tmp.parent;
            // push it on stack if it was a YES node
            if(parent.weight < tmp.weight) optimalItems.push(items[tmp.level]);
            tmp = parent;
        }

        //////////////////
        // Print output //
        //////////////////
        PrintWriter writer = new PrintWriter(System.out);
        if(args.length > 1) {
            System.err.println("\nPrinting output to " + args[1] + ".\n");
            writer = new PrintWriter(new File(args[1]), "UTF-8");
        }
        else System.err.println("\nNo output file specified, printing to standard out:\n------------------------------");
            
        writer.println(size + "," + maxProfit + "," + optimalSolSize);
        writer.println(visited + "," + leaves);
        while(!optimalItems.empty()) {  // pop optimal nodes off stack
            Item selectedItem = optimalItems.pop();
            writer.println(selectedItem.weight + "," + selectedItem.profit);
        }
        writer.close();
    }
}

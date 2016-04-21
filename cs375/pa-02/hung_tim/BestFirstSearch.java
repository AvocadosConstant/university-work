import java.io.File;
import java.io.IOException;
import java.util.Scanner;
import java.util.Comparator;
import java.util.PriorityQueue;
import java.util.Arrays;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Stack;

public class BestFirstSearch {

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
        }
        return bound;
    }

    public static void main(String[] args) throws IOException {
        Item[] items = new Item[1];
        int capacity = 0; //knapsack capacity
        int maxProfit = 0;
        int optimalSolSize = 0;
        int visited = 1;
        int leaves = 0;
        int size = 0;
        Node finalNode = null;

        try (Scanner scanner = new Scanner(new File(args[0]))) {
            String[] vals = scanner.nextLine().split(",");
            size = Integer.parseInt(vals[0]);
            capacity = Integer.parseInt(vals[1]);
            
            items = new Item[size];

            System.out.println("size = " +  Integer.parseInt(vals[0]));
            System.out.println("capacity = " + Integer.parseInt(vals[1]));

            for(int i = 0; i < size; i++) {
                vals = scanner.nextLine().split(",");
                items[i] = new Item(Integer.parseInt(vals[0]), Integer.parseInt(vals[1]));
            }
        } catch (IOException e) {
            e.printStackTrace();
        }

        Arrays.sort(items);
        System.out.println("Items:");
        for(Item i : items) {
            System.out.print("    weight = " + i.weight);
            System.out.print(" | profit = " + i.profit);
            System.out.println(" | ratio = " + i.ratio);
        }
        NodeComparator comparator = new NodeComparator();
        PriorityQueue<Node> prioQ = new PriorityQueue<Node>(comparator); 

        Node root = new Node(null, -1, 0, 0, bound(items, 0, 0, 0, capacity));
        prioQ.add(root);

        while(prioQ.size() > 0) {
            Node focus = prioQ.poll();
            //System.out.println("focus:\t\t" + focus);
            if(focus.bound > maxProfit) {
                int level = focus.level + 1;
                Item curItem = items[focus.level + 1];

                double yesBound = bound(items, level+1, focus.weight + curItem.weight, focus.profit + curItem.profit, capacity);
                Node yes = new Node(focus, level, focus.profit + curItem.profit, focus.weight + curItem.weight, yesBound);
                System.out.println("YES:\t\t" + yes);
                visited++;
                if(yes.level + 1 == items.length) leaves++;

                if(yes.weight <= capacity && yes.profit > maxProfit) {
                    maxProfit = yes.profit;
                    optimalSolSize = yes.level;
                    finalNode = yes;
                    if(yes.bound > maxProfit) prioQ.add(yes);
                }

                double noBound = bound(items, level+1, focus.weight, focus.profit, capacity);
                Node no = new Node(focus, level, focus.profit, focus.weight, noBound);
                System.out.println("NO:\t\t" + no);
                visited++;
                if(no.level + 1 == items.length) leaves++;

                if(no.bound > maxProfit) prioQ.add(no);
            }
        }

        System.out.println("Max profit: " + maxProfit);
        System.out.println("Visited: " + visited);
        System.out.println("Leaves: " + leaves);

        Stack<Item> optimalItems = new Stack<Item>();
        System.out.println("\nFinal Node: " + finalNode);
        Node tmp = finalNode;
        while(tmp.parent != null) {
            Node parent = tmp.parent;
            if(parent.weight < tmp.weight) optimalItems.push(items[tmp.level]);
            tmp = parent;
        }

        File outfile = new File(args[1]);
        PrintWriter writer = new PrintWriter(outfile, "UTF-8");
    
        writer.println(size + "," + maxProfit + "," + optimalSolSize);
        writer.println(visited + "," + leaves);
        while(!optimalItems.empty()) {
            Item selectedItem = optimalItems.pop();
            writer.println(selectedItem.weight + "," + selectedItem.profit);
        }

        writer.close();
    }
}

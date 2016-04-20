import java.util.*;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.stream.Stream;
import java.io.File;
import java.io.IOException;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) {

        int size = 0;
        int capacity = 0;
        List<Item> items = new LinkedList<Item>();

        try (Scanner scanner = new Scanner(new File(args[0]))) {
            String[] vals = scanner.nextLine().split(",");
            size = Integer.parseInt(vals[0]);
            capacity = Integer.parseInt(vals[1]);
            
            System.out.println("size = " +  Integer.parseInt(vals[0]));
            System.out.println("capacity = " + Integer.parseInt(vals[1]));

            for(int i = 0; i < size; i++) {
                vals = scanner.nextLine().split(",");
                Item item = new Item();
                item.weight = Integer.parseInt(vals[0]);
                item.value = Integer.parseInt(vals[1]);

                System.out.print("item.weight = " + Integer.parseInt(vals[0]));
                System.out.print(" | item.value = " + Integer.parseInt(vals[1]));
                System.out.println();
            }

        } catch (IOException e) {
            e.printStackTrace();
        }

        KnapsackSolver solver = new BranchAndBoundSolver(items, capacity);

        System.out.println(solver.solve());
    }
}

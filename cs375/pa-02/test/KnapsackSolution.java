import java.util.*;

public class KnapsackSolution {
   
   String approach;
   public List<Item> items;
   public double weight;
   public double value;
   
   @Override
   public String toString() {
      StringBuilder builder = new StringBuilder();
      builder.append(approach);
      builder.append(": ");
      builder.append(value);
      builder.append(" ");
      builder.append(weight);
      builder.append("\n");
      
      return builder.toString();
   }
}
